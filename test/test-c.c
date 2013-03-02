
#include <gsound.h>

#include <canberra.h>

static void
print_error(GError *error)
{
    if (!error)
        return;
    
    g_critical("Received error:"
               "\tdomain: %d\n"
               "\tcode: %d\n"
               "\tmessage: %s",
               error->domain, error->code, error->message);
    g_error_free(error);
}

int
main(int argc, char *argv[])
{
    GError *error = NULL;
    
    g_type_init();

    GSoundContext *ctx = gsound_context_new(NULL, &error);
    if (error)
    {
        print_error(error);
        return -1;
    }

    return 0;
}