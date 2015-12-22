#include "ocr.h"

ocrGuid_t f(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    const u64 n = paramv[0];
    PRINTF("Hello from %lu\n", n);
    return NULL_GUID;
}

ocrGuid_t realMainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[])
{
  int i;
  
  PRINTF("Hello from realMainEdt()\n");

  ocrGuid_t template, edt, event;
  ocrEventCreate(&event, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
  ocrEdtTemplateCreate(&template, f, 1, 1);

  for(i=0;i<10;i++)
  {
    u64 n=i%2;

    ocrEdtCreate(&edt, template, 1, &n, 1, &event, EDT_PROP_NONE, NULL_GUID, NULL);
    { //OCR hints
      ocrHint_t _stepHints;
      ocrHintInit(&_stepHints, OCR_HINT_EDT_T);
      u64 _hintVal=n;
      ocrSetHintValue(&_stepHints, OCR_HINT_EDT_IS_STOKER, _hintVal);
      ocrSetHint(edt, &_stepHints);
    }
    PRINTF("CREATE %lu\n", n);
  }

  ocrEventSatisfy(event, NULL_GUID);

  return NULL_GUID;
}

ocrGuid_t wrapupEdt()
{
  PRINTF("shutting down\n");
  ocrShutdown();
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    u64 i;

    PRINTF("Hello from mainEdt()\n");

    ocrGuid_t realMain, realMainTemplate, output, wrapup, wrapupTemplate, event;

    ocrEventCreate(&event, OCR_EVENT_ONCE_T, EVT_PROP_NONE);

    ocrEdtTemplateCreate(&wrapupTemplate, wrapupEdt, 0, 1);
    ocrEdtTemplateCreate(&realMainTemplate, realMainEdt, 0, 1);

    ocrEdtCreate(&realMain, realMainTemplate, EDT_PARAM_DEF, NULL, 1, &event, EDT_PROP_FINISH, NULL_GUID, &output);
    ocrEdtCreate(&wrapup, wrapupTemplate, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, NULL, EDT_PROP_FINISH, NULL_GUID, NULL_GUID);

    ocrAddDependence(output, wrapup, 0, DB_MODE_RW);

    ocrEventSatisfy(event, NULL_GUID);

    return NULL_GUID;
}
