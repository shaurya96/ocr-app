#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cstdlib>
#include "lulesh.h"

/////////////////////////////////////////////////////////////////////
Domain::Domain(Index_t nx, int nr, int balance, Int_t cost)
:
		m_e_cut(Real_t(1.0e-7)),
		m_p_cut(Real_t(1.0e-7)),
		m_q_cut(Real_t(1.0e-7)),
		m_v_cut(Real_t(1.0e-10)),
		m_u_cut(Real_t(1.0e-7)),
		m_hgcoef(Real_t(3.0)),
		m_ss4o3(Real_t(4.0)/Real_t(3.0)),
		m_qstop(Real_t(1.0e+12)),
		m_monoq_max_slope(Real_t(1.0)),
		m_monoq_limiter_mult(Real_t(2.0)),
		m_qlc_monoq(Real_t(0.5)),
		m_qqc_monoq(Real_t(2.0)/Real_t(3.0)),
		m_qqc(Real_t(2.0)),
		m_eosvmax(Real_t(1.0e+9)),
		m_eosvmin(Real_t(1.0e-9)),
		m_pmin(Real_t(0.)),
		m_emin(Real_t(-1.0e+15)),
		m_dvovmax(Real_t(0.1)),
		m_refdens(Real_t(1.0))
{

	Index_t edgeElems = nx;
	Index_t edgeNodes = edgeElems+1 ;
	this->cost() = cost;

	///////////////////////////////
	//   Initialize Sedov Mesh
	///////////////////////////////

	// construct a uniform box for this processor
	m_sizeX = edgeElems ;
	m_sizeY = edgeElems ;
	m_sizeZ = edgeElems ;
	m_numElem = edgeElems*edgeElems*edgeElems ;

	m_numNode = edgeNodes*edgeNodes*edgeNodes ;

	m_regNumList = new Index_t[numElem()] ;  // material indexset

	// Elem-centered
	AllocateElemPersistent(numElem()) ;

	// Node-centered
	AllocateNodePersistent(numNode()) ;

	SetupCommBuffers(edgeNodes);

	// Basic Field Initialization
	for (Index_t i=0; i<numElem(); ++i) {
		e(i) =  Real_t(0.0) ;
		p(i) =  Real_t(0.0) ;
		q(i) =  Real_t(0.0) ;
		ss(i) = Real_t(0.0) ;
	}

	// Note - v initializes to 1.0, not 0.0!
	for (Index_t i=0; i<numElem(); ++i) {
		v(i) = Real_t(1.0) ;
	}

	for (Index_t i=0; i<numNode(); ++i) {
		xd(i) = Real_t(0.0) ;
		yd(i) = Real_t(0.0) ;
		zd(i) = Real_t(0.0) ;
	}

	for (Index_t i=0; i<numNode(); ++i) {
		xdd(i) = Real_t(0.0) ;
		ydd(i) = Real_t(0.0) ;
		zdd(i) = Real_t(0.0) ;
	}

	for (Index_t i=0; i<numNode(); ++i) {
		nodalMass(i) = Real_t(0.0) ;
	}

	BuildMesh(nx, edgeNodes, edgeElems);

	// Setup region index sets. For now, these are constant sized
	// throughout the run, but could be changed every cycle to
	// simulate effects of ALE on the lagrange solver
	CreateRegionIndexSets(nr, balance);

	// Setup symmetry nodesets
	SetupSymmetryPlanes(edgeNodes);

	// Setup element connectivities
	SetupElementConnectivities(edgeElems);

	// Setup symmetry planes and free surface boundary arrays
	SetupBoundaryConditions(edgeElems);


	// Setup defaults

	// These can be changed (requires recompile) if you want to run
	// with a fixed timestep, or to a different end time, but it's
	// probably easier/better to just run a fixed number of timesteps
	// using the -i flag in 2.x

	dtfixed() = Real_t(-1.0e-6) ; // Negative means use courant condition
	stoptime()  = Real_t(1.0e-2); // *Real_t(edgeElems*tp/45.0) ;

	// Initial conditions
	deltatimemultlb() = Real_t(1.1) ;
	deltatimemultub() = Real_t(1.2) ;
	dtcourant() = Real_t(1.0e+20) ;
	dthydro()   = Real_t(1.0e+20) ;
	dtmax()     = Real_t(1.0e-2) ;
	time()    = Real_t(0.) ;
	cycle()   = Int_t(0) ;

	// initialize field data
	for (Index_t i=0; i<numElem(); ++i) {
		Real_t x_local[8], y_local[8], z_local[8] ;
		Index_t *elemToNode = nodelist(i) ;
		for( Index_t lnode=0 ; lnode<8 ; ++lnode )
		{
			Index_t gnode = elemToNode[lnode];
			x_local[lnode] = x(gnode);
			y_local[lnode] = y(gnode);
			z_local[lnode] = z(gnode);
		}

		// volume calculations
		Real_t volume = CalcElemVolume(x_local, y_local, z_local );
		volo(i) = volume ;
		elemMass(i) = volume ;
		for (Index_t j=0; j<8; ++j) {
			Index_t idx = elemToNode[j] ;
			nodalMass(idx) += volume / Real_t(8.0) ;
		}
	}

	// deposit initial energy
	// An energy of 3.948746e+7 is correct for a problem with
	// 45 zones along a side - we need to scale it
	const Real_t ebase = Real_t(3.948746e+7);
	Real_t scale = (nx)/Real_t(45.0);
	Real_t einit = ebase*scale*scale*scale;

	e(0) = einit;

	//set initial deltatime base on analytic CFL calculation
	deltatime() = (Real_t(.5)*cbrt(volo(0)))/sqrt(Real_t(2.0)*einit);

} // End constructor


////////////////////////////////////////////////////////////////////////////////
void
Domain::BuildMesh(Int_t nx, Int_t edgeNodes, Int_t edgeElems)
{
	Index_t meshEdgeElems = nx ;

	// initialize nodal coordinates
	Index_t nidx = 0 ;
	Real_t tz = Real_t(1.125)*Real_t(0*nx)/Real_t(meshEdgeElems) ;
	for (Index_t plane=0; plane<edgeNodes; ++plane) {
		Real_t ty = Real_t(1.125)*Real_t(0*nx)/Real_t(meshEdgeElems) ;
		for (Index_t row=0; row<edgeNodes; ++row) {
			Real_t tx = Real_t(1.125)*Real_t(0*nx)/Real_t(meshEdgeElems) ;
			for (Index_t col=0; col<edgeNodes; ++col) {
				x(nidx) = tx ;
				y(nidx) = ty ;
				z(nidx) = tz ;
				++nidx ;
				// tx += ds ; // may accumulate roundoff...
				tx = Real_t(1.125)*Real_t(col+1)/Real_t(meshEdgeElems) ;
			}
			// ty += ds ;  // may accumulate roundoff...
			ty = Real_t(1.125)*Real_t(row+1)/Real_t(meshEdgeElems) ;
		}
		// tz += ds ;  // may accumulate roundoff...
		tz = Real_t(1.125)*Real_t(plane+1)/Real_t(meshEdgeElems) ;
	}


	// embed hexehedral elements in nodal point lattice
	Index_t zidx = 0 ;
	nidx = 0 ;
	for (Index_t plane=0; plane<edgeElems; ++plane) {
		for (Index_t row=0; row<edgeElems; ++row) {
			for (Index_t col=0; col<edgeElems; ++col) {
				Index_t *localNode = nodelist(zidx) ;
				localNode[0] = nidx                                       ;
				localNode[1] = nidx                                   + 1 ;
				localNode[2] = nidx                       + edgeNodes + 1 ;
				localNode[3] = nidx                       + edgeNodes     ;
				localNode[4] = nidx + edgeNodes*edgeNodes                 ;
				localNode[5] = nidx + edgeNodes*edgeNodes             + 1 ;
				localNode[6] = nidx + edgeNodes*edgeNodes + edgeNodes + 1 ;
				localNode[7] = nidx + edgeNodes*edgeNodes + edgeNodes     ;
				++zidx ;
				++nidx ;
			}
			++nidx ;
		}
		nidx += edgeNodes ;
	}
}

////////////////////////////////////////////////////////////////////////////////
void
Domain::SetupCommBuffers(Int_t edgeNodes)
{
	// allocate a buffer large enough for nodal ghost data
	Index_t maxEdgeSize = MAX(this->sizeX(), MAX(this->sizeY(), this->sizeZ()))+1 ;
	m_maxPlaneSize = CACHE_ALIGN_REAL(maxEdgeSize*maxEdgeSize) ;
	m_maxEdgeSize = CACHE_ALIGN_REAL(maxEdgeSize) ;

	// assume communication to 6 neighbors by default
	m_rowMin =  0;
	m_rowMax = 0;
	m_colMin = 0;
	m_colMax = 0;
	m_planeMin = 0;
	m_planeMax = 0;

	// Boundary nodesets
	m_symmX.resize(edgeNodes*edgeNodes);
	m_symmY.resize(edgeNodes*edgeNodes);
	m_symmZ.resize(edgeNodes*edgeNodes);
}


////////////////////////////////////////////////////////////////////////////////
void
Domain::CreateRegionIndexSets(Int_t nr, Int_t balance)
{
	srand(0);
	this->numReg() = nr;
	m_regElemSize = new Index_t[numReg()];
	m_regElemlist = new Index_t*[numReg()];
	Index_t nextIndex = 0;
	//if we only have one region just fill it
	// Fill out the regNumList with material numbers, which are always
	// the region index plus one
	if(numReg() == 1) {
		while (nextIndex < numElem()) {
			this->regNumList(nextIndex) = 1;
			nextIndex++;
		}
		regElemSize(0) = 0;
	}
	//If we have more than one region distribute the elements.
	else {
		Int_t regionNum;
		Int_t regionVar;
		Int_t lastReg = -1;
		Int_t binSize;
		Index_t elements;
		Index_t runto = 0;
		Int_t costDenominator = 0;
		Int_t* regBinEnd = new Int_t[numReg()];
		//Determine the relative weights of all the regions.  This is based off the -b flag.  Balance is the value passed into b.
		for (Index_t i=0 ; i<numReg() ; ++i) {
			regElemSize(i) = 0;
			costDenominator += pow((i+1), balance);  //Total sum of all regions weights
			regBinEnd[i] = costDenominator;  //Chance of hitting a given region is (regBinEnd[i] - regBinEdn[i-1])/costDenominator
		}
		//Until all elements are assigned
		while (nextIndex < numElem()) {
			//pick the region
			regionVar = rand() % costDenominator;
			Index_t i = 0;
			while(regionVar >= regBinEnd[i])
				i++;
			// Rotate the regions based i % NumRegions this makes each domain have a different region with
			//the highest representation
			regionNum = (i % numReg()) + 1;
			// make sure we don't pick the same region twice in a row
			while(regionNum == lastReg) {
				regionVar = rand() % costDenominator;
				i = 0;
				while(regionVar >= regBinEnd[i])
					i++;
				regionNum = (i % numReg()) + 1;
			}
			//Pick the bin size of the region and determine the number of elements.
			binSize = rand() % 1000;
			if(binSize < 773) {
				elements = rand() % 15 + 1;
			}
			else if(binSize < 937) {
				elements = rand() % 16 + 16;
			}
			else if(binSize < 970) {
				elements = rand() % 32 + 32;
			}
			else if(binSize < 974) {
				elements = rand() % 64 + 64;
			}
			else if(binSize < 978) {
				elements = rand() % 128 + 128;
			}
			else if(binSize < 981) {
				elements = rand() % 256 + 256;
			}
			else
				elements = rand() % 1537 + 512;
			runto = elements + nextIndex;
			//Store the elements.  If we hit the end before we run out of elements then just stop.
			while (nextIndex < runto && nextIndex < numElem()) {
				this->regNumList(nextIndex) = regionNum;
				nextIndex++;
			}
			lastReg = regionNum;
		}
	}
	// Convert regNumList to region index sets
	// First, count size of each region
	for (Index_t i=0 ; i<numElem() ; ++i) {
		int r = this->regNumList(i)-1; // region index == regnum-1
		regElemSize(r)++;
	}
	// Second, allocate each region index set
	for (Index_t i=0 ; i<numReg() ; ++i) {
		m_regElemlist[i] = new Index_t[regElemSize(i)];
		regElemSize(i) = 0;
	}
	// Third, fill index sets
	for (Index_t i=0 ; i<numElem() ; ++i) {
		Index_t r = regNumList(i)-1;       // region index == regnum-1
		Index_t regndx = regElemSize(r)++; // Note increment
		regElemlist(r,regndx) = i;
	}

}

/////////////////////////////////////////////////////////////
void 
Domain::SetupSymmetryPlanes(Int_t edgeNodes)
{
	Index_t nidx = 0 ;
	for (Index_t i=0; i<edgeNodes; ++i) {
		Index_t planeInc = i*edgeNodes*edgeNodes ;
		Index_t rowInc   = i*edgeNodes ;
		for (Index_t j=0; j<edgeNodes; ++j) {
			m_symmZ[nidx] = rowInc   + j ;
			m_symmY[nidx] = planeInc + j ;
			m_symmX[nidx] = planeInc + j*edgeNodes ;
			++nidx ;
		}
	}
}



/////////////////////////////////////////////////////////////
void
Domain::SetupElementConnectivities(Int_t edgeElems)
{
	lxim(0) = 0 ;
	for (Index_t i=1; i<numElem(); ++i) {
		lxim(i)   = i-1 ;
		lxip(i-1) = i ;
	}
	lxip(numElem()-1) = numElem()-1 ;

	for (Index_t i=0; i<edgeElems; ++i) {
		letam(i) = i ;
		letap(numElem()-edgeElems+i) = numElem()-edgeElems+i ;
	}
	for (Index_t i=edgeElems; i<numElem(); ++i) {
		letam(i) = i-edgeElems ;
		letap(i-edgeElems) = i ;
	}

	for (Index_t i=0; i<edgeElems*edgeElems; ++i) {
		lzetam(i) = i ;
		lzetap(numElem()-edgeElems*edgeElems+i) = numElem()-edgeElems*edgeElems+i ;
	}
	for (Index_t i=edgeElems*edgeElems; i<numElem(); ++i) {
		lzetam(i) = i - edgeElems*edgeElems ;
		lzetap(i-edgeElems*edgeElems) = i ;
	}
}

/////////////////////////////////////////////////////////////
void
Domain::SetupBoundaryConditions(Int_t edgeElems) 
{
	Index_t ghostIdx[6] ;  // offsets to ghost locations

	// set up boundary condition information
	for (Index_t i=0; i<numElem(); ++i) {
		elemBC(i) = Int_t(0) ;
	}

	for (Index_t i=0; i<6; ++i) {
		ghostIdx[i] = INT_MIN ;
	}

	Int_t pidx = numElem() ;
	if (m_planeMin != 0) {
		ghostIdx[0] = pidx ;
		pidx += sizeX()*sizeY() ;
	}

	if (m_planeMax != 0) {
		ghostIdx[1] = pidx ;
		pidx += sizeX()*sizeY() ;
	}

	if (m_rowMin != 0) {
		ghostIdx[2] = pidx ;
		pidx += sizeX()*sizeZ() ;
	}

	if (m_rowMax != 0) {
		ghostIdx[3] = pidx ;
		pidx += sizeX()*sizeZ() ;
	}

	if (m_colMin != 0) {
		ghostIdx[4] = pidx ;
		pidx += sizeY()*sizeZ() ;
	}

	if (m_colMax != 0) {
		ghostIdx[5] = pidx ;
	}

	// symmetry plane or free surface BCs
	for (Index_t i=0; i<edgeElems; ++i) {
		Index_t planeInc = i*edgeElems*edgeElems ;
		Index_t rowInc   = i*edgeElems ;
		for (Index_t j=0; j<edgeElems; ++j) {
			elemBC(rowInc+j) |= ZETA_M_SYMM ;

			elemBC(rowInc+j+numElem()-edgeElems*edgeElems) |= ZETA_P_FREE;

			elemBC(planeInc+j) |= ETA_M_SYMM ;

			elemBC(planeInc+j+edgeElems*edgeElems-edgeElems) |= ETA_P_FREE ;

			elemBC(planeInc+j*edgeElems) |= XI_M_SYMM ;

			elemBC(planeInc+j*edgeElems+edgeElems-1) |= XI_P_FREE ;
		}
	}
}
