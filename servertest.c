#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "open62541.h"
int count = 0;
int i = 0;

static void
addToArray(UA_Server *server, int *count, UA_String *motor, UA_String *values) {

    values[i] = UA_String_fromChars(motor);
    i++;

}

static void
    readArray()
{
    
}

static void
createarray(UA_Server *server, int *count, UA_String *motor) {
    // Attributes for variable node
    UA_VariableAttributes vattr = UA_VariableAttributes_default;

    const size_t array_size = count;
    UA_String *values = (UA_String *)UA_Array_new(array_size, &UA_TYPES[UA_TYPES_STRING]);

    addToArray(server, count, motor, values);

    UA_Variant_setArray(&vattr.value, values, array_size, &UA_TYPES[UA_TYPES_STRING]);
    vattr.valueRank = UA_VALUERANK_ANY;

    UA_UInt32 myArrayDimensions[1] = {array_size};
    vattr.value.arrayDimensions = myArrayDimensions;
    vattr.value.arrayDimensionsSize = 1;
    vattr.displayName = UA_LOCALIZEDTEXT("en-US", "Tablica MotorIDs");
    UA_StatusCode retval = UA_Server_addVariableNode(
        server, UA_NODEID_NUMERIC(1, 1),               // new node id
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),  // parent node
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),      // reference type
        UA_QUALIFIEDNAME(1, "MotorID"),                // node browse name
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vattr, NULL, NULL);
    if(count == 2)
    {
        
        
    }
    //UA_Variant_setArray(&vattr.value, values, array_size, &UA_TYPES[UA_TYPES_STRING]);
    //vattr.valueRank = UA_VALUERANK_ANY;

    //UA_UInt32 myArrayDimensions[1] = {array_size};
    //vattr.value.arrayDimensions = myArrayDimensions;
    //vattr.value.arrayDimensionsSize = 1;
    //vattr.displayName = UA_LOCALIZEDTEXT("en-US", "Tablica MotorIDs");
    //UA_StatusCode retval = UA_Server_addVariableNode(
    //    server, UA_NODEID_STRING(1, "ID"),  // new node id
    //    UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),  // parent node
    //    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),      // reference type
    //    UA_QUALIFIEDNAME(1, "MotorID"),                // node browse name
    //    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vattr, NULL, NULL);

    //if(retval != UA_STATUSCODE_GOOD) {
    //    // handle error
    //    printf("Error adding node");
    //}


    // clean up
    //UA_Array_delete(values, array_size, &UA_TYPES[UA_TYPES_STRING]);
}

static void
manuallyDefinePump(UA_Server *server, char *pumpname, char *mnname, char *modelname, char *motor) {
    UA_NodeId pumpId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", pumpname);
    UA_Server_addObjectNode(
        server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, "Pump (Manual)"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), oAttr, NULL, &pumpId);

    UA_VariableAttributes mnAttr = UA_VariableAttributes_default;
    UA_String manufacturerName = UA_STRING(mnname);
    UA_Variant_setScalar(&mnAttr.value, &manufacturerName, &UA_TYPES[UA_TYPES_STRING]);
    mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "ManufacturerName"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), mnAttr, NULL, NULL);

    UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
    UA_String modelName = UA_STRING(modelname);
    UA_Variant_setScalar(&modelAttr.value, &modelName, &UA_TYPES[UA_TYPES_STRING]);
    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ModelName");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "ModelName"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), modelAttr, NULL, NULL);

    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    UA_Boolean status = true;
    UA_Variant_setScalar(&statusAttr.value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    statusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Status"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, NULL);

    UA_VariableAttributes rpmAttr = UA_VariableAttributes_default;
    UA_NodeId rpmNodeId = UA_NODEID_STRING(1, motor);
    UA_Double rpm = 0;
    UA_Variant_setScalar(&rpmAttr.value, &rpm, &UA_TYPES[UA_TYPES_DOUBLE]);
    rpmAttr.displayName = UA_LOCALIZEDTEXT("en-US", "MotorRPM");
    UA_Server_addVariableNode(
        server, rpmNodeId, pumpId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "MotorRPMs"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), rpmAttr, NULL, NULL);
    count++;
    //printf("COUNT == %d", count);
    createarray(server, count, motor);
}



static void
writeVariable(UA_Server *server, void *data, char *motor) {

    UA_NodeId rpmNodeId = UA_NODEID_STRING(1, "MotorRPM1");
    UA_NodeId rpmNodeId2 = UA_NODEID_STRING(1, "MotorRPM2");
    // time_t timerand;
    // srand((unsigned)time(&timerand));
    // srand(time(NULL));
    UA_Double rpm = (double)rand() / RAND_MAX * 100.0;
    UA_Double rpm2 = (double)rand() / RAND_MAX * 100.0;
    round(rpm);
    round(rpm2);

    //printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);
    if(rpm >= 85 || rpm2 >= 85) {
        //printf("Osiagnieto zbyt wysoki poziom obrotow pompy!\nZwalnianie zaworow\n...\n");
        rpm = rpm - 20.00;
        rpm2 = rpm2 - 20.00;
        UA_sleep_ms(1000);
        //printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);

        UA_sleep_ms(1000);
    }

    if(rpm <= 15 || rpm2 <= 15) {
        //printf("Krytycznie niski poziom obrotow pompy!\nZwiekszanie cisnienia\n...\n");
        rpm = rpm + 20.00;
        rpm2 = rpm2 + 20.00;
        UA_sleep_ms(1000);
        //printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);

        UA_sleep_ms(1000);
    }

    /* Write a different integer value */
    UA_VariableAttributes rpmAttr, rpmAttr2;
    UA_Variant_init(&rpmAttr.value);
    UA_Variant_init(&rpmAttr2.value);
    UA_Variant_setScalar(&rpmAttr.value, &rpm, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Variant_setScalar(&rpmAttr2.value, &rpm2, &UA_TYPES[UA_TYPES_DOUBLE]);

    // Use a more detailed write function than UA_Server_writeValue
    UA_WriteValue wv;
    UA_WriteValue_init(&wv);
    wv.nodeId = rpmNodeId;
    wv.attributeId = UA_ATTRIBUTEID_VALUE;
    wv.value.hasStatus = false;
    wv.value.value = rpmAttr.value;
    wv.value.hasValue = true;
    
    UA_WriteValue wv2;
    UA_WriteValue_init(&wv2);
    wv2.nodeId = rpmNodeId2;
    wv2.attributeId = UA_ATTRIBUTEID_VALUE;
    wv2.value.hasStatus = false;
    wv2.value.value = rpmAttr2.value;
    wv2.value.hasValue = true;

    UA_DateTime currentTime = UA_DateTime_now();
    wv.value.hasSourceTimestamp = true;
    wv.value.sourceTimestamp = 1 * UA_DATETIME_SEC;

    UA_Server_writeDataValue(server, rpmNodeId, wv.value);
    UA_Server_writeDataValue(server, rpmNodeId2, wv2.value);
}

/** It follows the main server code, making use of the above definitions. */

static volatile UA_Boolean running = true;
static void
stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Zatrzymano serwer");
    running = false;
}

int
main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    manuallyDefinePump(server, "Pompa Manual - Dwor", "LitPumps Ltd.", "WoIK-V17", "MotorRPM1");
    manuallyDefinePump(server, "Pompa Manual - Dwor2", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");
    //manuallyDefinePump(server, "Pompa Manual - Dwor3", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");
    //manuallyDefinePump(server, "Pompa Manual - Dwor4", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");
    //manuallyDefinePump(server, "Pompa Manual - Dwor5", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");
    //manuallyDefinePump(server, "Pompa Manual - Dwor6", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");
    //manuallyDefinePump(server, "Pompa Manual - Dwor7", "LitPumps Ltd.", "WoIK-V17", "MotorRPM2");

    //createarray(server, 3, "motor");
    //createarray(server, 5, "motorynka");
    //createarray(server, 10, "jablko");


    UA_Server_addRepeatedCallback(server, writeVariable, NULL, 2000, NULL);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
