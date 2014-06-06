
//
// This file is part of the CNC-C implementation and
// distributed under the Modified BSD License.
// See LICENSE for details.
//
// I AM A GENERATED FILE. PLEASE DO NOT CHANGE ME!!!
//

package CnCParser.Ast;

import lpg.runtime.*;

/**
 * is implemented by:
 *<b>
 *<ul>
 *<li>StepExecution
 *<li>StepPrescription
 *</ul>
 *</b>
 */
public interface Irelation
{
    public IToken getLeftIToken();
    public IToken getRightIToken();

    void accept(IAstVisitor v);
}


