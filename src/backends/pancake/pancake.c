#include <stdio.h>

#include "../../analyse/data/kludge.h"
#include "../../data/dict.h"
#include "data/instructions.h"
#include "data/runtime_data.h"
#include "pancake.h"

/* pancake - stack based interpreter backend
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake(struct ic_kludge *kludge) {
    struct ic_backend_pancake_instructions *instructions = 0;
    struct ic_backend_pancake_runtime_data *runtime_data = 0;

    if (!kludge) {
        puts("ic_backend_pancake: kludge was null");
        return 0;
    }

    instructions = ic_backend_pancake_compile(kludge);
    if (!instructions) {
        puts("ic_backend_pancake: call to ic_backend_pancake_compile failed");
        return 0;
    }

    /* print instructions */
    puts("Pancake bytecode:");
    puts("==========================");
    if (!ic_backend_pancake_instructions_print(instructions, stdout)) {
        puts("ic_backend_pancake: call to ic_backend_pancake_instructions_print failed");
        return 0;
    }
    puts("==========================");
    puts("");

    puts("Pancake interpreter output");
    puts("==========================");

    runtime_data = ic_backend_pancake_runtime_data_new(instructions);
    if (!runtime_data) {
        puts("ic_backend_pancake: call to ic_backend_pancake_runtime_data_new failed");
        return 0;
    }

    if (!ic_backend_pancake_interpret(runtime_data)) {
        puts("ic_backend_pancake: call to ic_backend_pancake_interpret failed");
        return 0;
    }

    puts("ic_backend_pancake: unimplemented");
    return 1;
}
