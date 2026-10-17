// Smoke test: the public header must compile as C++ for a consumer.
#include "lensfun.h"

int main()
{
    lfDatabase *db = lf_db_create();
    if (db)
    {
        lf_db_destroy(db);
    }
    return 0;
}
