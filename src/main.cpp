#include "BookSystem.h"
#include "Bookstore.h"
#include "AccountSystem.h"
#include "LogSystem.h"
#include "KV_Database.h"

int main() {
    Bookstore MyStore;
    while (true) {
        try { MyStore.work(); }
        catch (const char *a) { cout << a << "\n"; }
        catch (int) {break;}
    }
    return 0;
}
