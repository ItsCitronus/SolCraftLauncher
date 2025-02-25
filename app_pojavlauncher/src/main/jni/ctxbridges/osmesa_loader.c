//
// Modifile by Vera-Firefly on 13.01.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "environ/environ.h"
#include "osmesa_loader.h"
#include "renderer_config.h"

GLboolean (*OSMesaMakeCurrent_p) (OSMesaContext ctx, void *buffer, GLenum type,
                                         GLsizei width, GLsizei height);
OSMesaContext (*OSMesaGetCurrentContext_p) (void);
OSMesaContext  (*OSMesaCreateContext_p) (GLenum format, OSMesaContext sharelist);
void (*OSMesaDestroyContext_p) (OSMesaContext ctx);
void (*OSMesaPixelStore_p) ( GLint pname, GLint value );
GLubyte* (*glGetString_p) (GLenum name);
void (*glFinish_p) (void);
void (*glClearColor_p) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (*glClear_p) (GLbitfield mask);
void (*glReadPixels_p) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * data);

void dlsym_OSMesa() {
    char* main_osm_libname = getenv("POJAVEXEC_OSMESA");
    char* main_path = NULL;
    if(pojav_environ->config_renderer == RENDERER_VK_ZINK) {
        if(main_osm_libname == NULL) main_osm_libname = "libOSMesa.so";
        if(asprintf(&main_path, "%s/%s", getenv("POJAV_NATIVEDIR"), main_osm_libname) == -1) {
            abort();
        }
        free(main_path);
    } else if(pojav_environ->config_renderer == RENDERER_VIRGL) {
        if(asprintf(&main_path, "%s/libOSMesa_znL.so", getenv("POJAV_NATIVEDIR")) == -1) {
            abort();
        }
    }
    void* dl_handle = NULL;
    dl_handle = dlopen(main_path, RTLD_GLOBAL);
    if(dl_handle == NULL) abort();
    OSMesaMakeCurrent_p = dlsym(dl_handle, "OSMesaMakeCurrent");
    OSMesaGetCurrentContext_p = dlsym(dl_handle,"OSMesaGetCurrentContext");
    OSMesaCreateContext_p = dlsym(dl_handle, "OSMesaCreateContext");
    OSMesaDestroyContext_p = dlsym(dl_handle, "OSMesaDestroyContext");
    OSMesaPixelStore_p = dlsym(dl_handle,"OSMesaPixelStore");
    glGetString_p = dlsym(dl_handle,"glGetString");
    glClearColor_p = dlsym(dl_handle, "glClearColor");
    glClear_p = dlsym(dl_handle,"glClear");
    glFinish_p = dlsym(dl_handle,"glFinish");
    glReadPixels_p = dlsym(dl_handle,"glReadPixels");
}
