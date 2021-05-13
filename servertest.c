#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "open62541.h"

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

    // Second manuall pump
/*
    UA_NodeId pumpId2;  get the nodeid assigned by the server 
    UA_ObjectAttributes o2Attr = UA_ObjectAttributes_default;
    o2Attr.displayName = UA_LOCALIZEDTEXT("en-US", "Pompa - Manual.2");
    UA_Server_addObjectNode(
        server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, "Pump (Manual-2)"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), o2Attr, NULL, &pumpId2);

    UA_VariableAttributes mn2Attr = UA_VariableAttributes_default;
    UA_String manufacturerName2 = UA_STRING("Pump King Ltd.");
    UA_Variant_setScalar(&mn2Attr.value, &manufacturerName2, &UA_TYPES[UA_TYPES_STRING]);
    mn2Attr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId2, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "ManufacturerName"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), mn2Attr, NULL, NULL);

    UA_VariableAttributes model2Attr = UA_VariableAttributes_default;
    UA_String modelName2 = UA_STRING("Mega Pump 3000");
    UA_Variant_setScalar(&model2Attr.value, &modelName2, &UA_TYPES[UA_TYPES_STRING]);
    model2Attr.displayName = UA_LOCALIZEDTEXT("en-US", "ModelName");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId2, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "ModelName"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), model2Attr, NULL, NULL);

    UA_VariableAttributes status2Attr = UA_VariableAttributes_default;
    UA_Boolean status2 = true;
    UA_Variant_setScalar(&status2Attr.value, &status2, &UA_TYPES[UA_TYPES_BOOLEAN]);
    status2Attr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpId2, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Status"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), status2Attr, NULL, NULL);

    UA_VariableAttributes rpm2Attr = UA_VariableAttributes_default;
    UA_NodeId rpmNodeId2 = UA_NODEID_STRING(1, "2MotorRPM");
    UA_Double rpm2 = 0;
    UA_Variant_setScalar(&rpm2Attr.value, &rpm2, &UA_TYPES[UA_TYPES_DOUBLE]);
    rpm2Attr.displayName = UA_LOCALIZEDTEXT("en-US", "MotorRPM");
    UA_Server_addVariableNode(
        server, rpmNodeId2, pumpId2, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "MotorRPMs"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), rpm2Attr, NULL, NULL);
    */
}

UA_NodeId pumpTypeId = {1, UA_NODEIDTYPE_NUMERIC, {1001}};

static void
defineObjectTypes(UA_Server *server) {
    /* Define the object type for "Device" */
    UA_NodeId deviceTypeId; /* get the nodeid assigned by the server */
    UA_ObjectTypeAttributes dtAttr = UA_ObjectTypeAttributes_default;
    dtAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Nazwa pompy");
    UA_Server_addObjectTypeNode(
        server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, "Nazwa pompy"),
        dtAttr, NULL, &deviceTypeId);

    UA_VariableAttributes npAttr = UA_VariableAttributes_default;
    npAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Nazwa producenta");
    UA_NodeId manufacturerNameId;
    UA_Server_addVariableNode(server, UA_NODEID_NULL, deviceTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Nazwa producenta"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), npAttr,
                              NULL, &manufacturerNameId);
    /* Make the manufacturer name mandatory 
    UA_Server_addReference(
        server, manufacturerNameId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
    */

    UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Nazwa modelu");
    UA_NodeId modelId;
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Status"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), modelAttr, NULL, &modelId);
    /* Make the status variable mandatory 
    UA_Server_addReference(
        server, modelId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
        */

    /* Define the object type for "Pump" */
    UA_ObjectTypeAttributes ptAttr = UA_ObjectTypeAttributes_default;
    ptAttr.displayName = UA_LOCALIZEDTEXT("en-US", "PumpType");
    UA_Server_addObjectTypeNode(server, pumpTypeId, deviceTypeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "PumpType"), ptAttr, NULL, NULL); 

    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    statusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    statusAttr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId statusId;
    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "Status"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, &statusId);

    /* Make the status variable mandatory 
    UA_Server_addReference(
        server, statusId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
        */

    UA_VariableAttributes rpmAttr = UA_VariableAttributes_default;
    UA_Double rpm = 0;
    UA_Variant_setScalar(&rpmAttr.value, &rpm, &UA_TYPES[UA_TYPES_DOUBLE]);
    rpmAttr.displayName = UA_LOCALIZEDTEXT("en-US", "MotorRPM");
    rpmAttr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId rpmNodeId = UA_NODEID_STRING(1, "2MotorRPM");
    UA_Server_addVariableNode(
        server, rpmNodeId, pumpTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, "MotorRPMs"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), rpmAttr, NULL, &rpmNodeId);
}

static void
addPumpObjectInstance(UA_Server *server, char *name, char *model) {
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", name);
    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", model);
    UA_Server_addObjectNode(
        server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, name),
        pumpTypeId, /* this refers to the object type
                       identifier */
        oAttr, NULL, NULL);

    UA_Server_addVariableNode(
        server, UA_NODEID_NULL, pumpTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(1, model), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
        modelAttr, NULL, NULL);
}
/*
static UA_StatusCode
pumpTypeConstructor(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *typeId, void *typeContext, const UA_NodeId *nodeId,
                    void **nodeContext) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Utworzono nowa pompe");

    /* Find the NodeId of the status child variable
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = UA_QUALIFIEDNAME(1, "Status");

    UA_BrowsePath bp;
    UA_BrowsePath_init(&bp);
    bp.startingNode = *nodeId;
    bp.relativePath.elementsSize = 1;
    bp.relativePath.elements = &rpe;

    UA_BrowsePathResult bpr = UA_Server_translateBrowsePathToNodeIds(server, &bp);
    if(bpr.statusCode != UA_STATUSCODE_GOOD || bpr.targetsSize < 1)
        return bpr.statusCode;

    /* Set the status value
    UA_Boolean status = true;
    UA_Variant value;
    UA_Variant_setScalar(&value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, bpr.targets[0].targetId.nodeId, value);
    UA_BrowsePathResult_clear(&bpr);

    /* At this point we could replace the node context ..

    return UA_STATUSCODE_GOOD;
}
*/
/*
static void
addPumpTypeConstructor(UA_Server *server) {
    UA_NodeTypeLifecycle lifecycle;
    lifecycle.constructor = pumpTypeConstructor;
    lifecycle.destructor = NULL;
    UA_Server_setNodeTypeLifecycle(server, pumpTypeId, lifecycle);
}
*/
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

    printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);
    if(rpm >= 85 || rpm2 >= 85) {
        printf("Osiagnieto zbyt wysoki poziom obrotow pompy!\nZwalnianie zaworow\n...\n");
        rpm = rpm - 20.00;
        rpm2 = rpm2 - 20.00;
        UA_sleep_ms(1000);
        printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);

        UA_sleep_ms(1000);
    }

    if(rpm <= 15 || rpm2 <= 15) {
        printf("Krytycznie niski poziom obrotow pompy!\nZwiekszanie cisnienia\n...\n");
        rpm = rpm + 20.00;
        rpm2 = rpm2 + 20.00;
        UA_sleep_ms(1000);
        printf("Symulowany RPM:\n  >1. %.2f\n  >2. %.2f\n", rpm, rpm2);

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
    defineObjectTypes(server);
    //addPumpObjectInstance(server, "Pompa Dwor", "WoIK-17");
    //addPumpObjectInstance(server, "Pompa Dwor_Ziemna", "WoIK-Undr");
    //addPumpTypeConstructor(server);
    //addPumpObjectInstance(server, "Pompa Dwor_Ziemna2", "WoIK-Undr");
    //addPumpObjectInstance(server, "Pompa Wewnetrzna7", "WoIK-4");
    void Service_Browse(UA_Server * server, const UA_BrowseRequest *request,
                        UA_BrowseResponse *response);
    UA_Server_addRepeatedCallback(server, writeVariable, NULL, 2000, NULL);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
