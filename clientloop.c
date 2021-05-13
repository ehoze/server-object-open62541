#include "open62541.h"
// gcc -std=c99 myclient.c open62541.c -o client -lws2_32
#include <signal.h>
#include <stdlib.h>

UA_Boolean running = true;

static void
stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_CLIENT, "Zatrzymano program");
    running = 0;
}

int
main(void) {
    signal(SIGINT, stopHandler); /* catches ctrl-c */

    UA_Client *client = UA_Client_new();
    UA_ClientConfig *cc = UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(cc);

    /* default timeout is 5 seconds. Set it to 1 second here for demo */
    cc->timeout = 5000;

    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    UA_Variant value2; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value2);

    /* Endless loop reading the server time */
    while(running) {
        /* if already connected, this will return GOOD and do nothing */
        /* if the connection is closed/errored, the connection will be reset and then
         * reconnected */
        /* Alternatively you can also use UA_Client_getState to get the current state */
        UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:8080");
        UA_StatusCode status = UA_Client_connect(client, "opc.tcp://localhost:8080");
        status = UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, "the.answer"),
                                              &value2);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_CLIENT,
                         "Not connected. Retrying to connect in 2 second");
            /* The connect may timeout after 1 second (see above) or it may fail
             * immediately on network errors */
            /* E.g. name resolution errors or unreachable network. Thus there should be a
             * small sleep here */
            UA_sleep_ms(2000);
            continue;
        }

        /* NodeId of the variable holding the current time */
        const UA_NodeId nodeId =
            UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);

        if(status == UA_STATUSCODE_GOOD &&
           UA_Variant_hasScalarType(&value2, &UA_TYPES[UA_TYPES_INT32])) {
            printf("Wartosc zmiennej to: \n  > %i\n", *(UA_Int32 *)value2.data);
        }
        UA_Variant_clear(&value2);

        retval = UA_Client_readValueAttribute(client, nodeId, &value);
        if(retval == UA_STATUSCODE_BADCONNECTIONCLOSED) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_CLIENT,
                         "Connection was closed. Reconnecting ...");
            continue;
        }
        if(retval == UA_STATUSCODE_GOOD &&
           UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
            UA_DateTime raw_date = *(UA_DateTime *)value.data;
            UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
            // UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            printf("Informacje o czasie: \n  > Data: %u-%u-%u \n  > Godzina: "
                   "%u:%u:%u.%03u\n",
                   dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec,
                   dts.milliSec);
        }
        UA_Variant_clear(&value);

        UA_sleep_ms(1000);
    };

    /* Clean up */
    UA_Variant_clear(&value);
    UA_Variant_clear(&value2);
    UA_Client_delete(client); /* Disconnects the client internally */
    return EXIT_SUCCESS;
}
