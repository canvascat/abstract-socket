#if !defined(__linux__) && !defined(__APPLE__)
# error "Only Linux and macOS are supported"
#endif

#include <node_api.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <cassert>

namespace {

napi_value Socket(napi_env env, napi_callback_info info) {
    size_t argc = 0;
    napi_value args[0];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    int fd;
    int type = SOCK_STREAM;

    // Use platform-specific socket flags
#if defined(__linux__)
    type |= SOCK_NONBLOCK | SOCK_CLOEXEC;
    fd = socket(AF_UNIX, type, 0);
#elif defined(__APPLE__)
    fd = socket(AF_UNIX, type, 0);
    if (fd != -1) {
        // Set non-blocking and close-on-exec flags manually on macOS
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags != -1) {
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
        fcntl(fd, F_SETFD, FD_CLOEXEC);
    }
#endif

    if (fd == -1) {
        fd = -errno;
    }

    napi_value result;
    napi_create_int32(env, fd, &result);
    return result;
}

napi_value Bind(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    int32_t fd;
    napi_get_value_int32(env, args[0], &fd);

    size_t path_len;
    napi_get_value_string_utf8(env, args[1], nullptr, 0, &path_len);
    char* path = new char[path_len + 1];
    napi_get_value_string_utf8(env, args[1], path, path_len + 1, nullptr);

    int err = 0;

    if (path[0] != '\0') {
        err = -EINVAL;
        goto out;
    }

    if (path_len > sizeof(sockaddr_un::sun_path)) {
        err = -EINVAL;
        goto out;
    }

    sockaddr_un s;
    memset(&s, 0, sizeof(s));
    memcpy(s.sun_path, path, path_len);
    s.sun_family = AF_UNIX;

    if (bind(fd, reinterpret_cast<sockaddr*>(&s), sizeof(s)) != 0) {
        err = -errno;
    }

out:
    delete[] path;
    napi_value result;
    napi_create_int32(env, err, &result);
    return result;
}

napi_value Connect(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    int32_t fd;
    napi_get_value_int32(env, args[0], &fd);

    size_t path_len;
    napi_get_value_string_utf8(env, args[1], nullptr, 0, &path_len);
    char* path = new char[path_len + 1];
    napi_get_value_string_utf8(env, args[1], path, path_len + 1, nullptr);

    int err = 0;

    if (path[0] != '\0') {
        err = -EINVAL;
        goto out;
    }

    if (path_len > sizeof(sockaddr_un::sun_path)) {
        err = -EINVAL;
        goto out;
    }

    sockaddr_un s;
    memset(&s, 0, sizeof(s));
    memcpy(s.sun_path, path, path_len);
    s.sun_family = AF_UNIX;

    if (connect(fd, reinterpret_cast<sockaddr*>(&s), sizeof(s)) != 0) {
        err = -errno;
    }

out:
    delete[] path;
    napi_value result;
    napi_create_int32(env, err, &result);
    return result;
}

napi_value Close(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1];
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

    int32_t fd;
    napi_get_value_int32(env, args[0], &fd);

    int err = 0;
    if (close(fd)) {
        err = -errno;
    }

    napi_value result;
    napi_create_int32(env, err, &result);
    return result;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_value socket_fn, bind_fn, connect_fn, close_fn;
    
    napi_create_function(env, nullptr, 0, Socket, nullptr, &socket_fn);
    napi_create_function(env, nullptr, 0, Bind, nullptr, &bind_fn);
    napi_create_function(env, nullptr, 0, Connect, nullptr, &connect_fn);
    napi_create_function(env, nullptr, 0, Close, nullptr, &close_fn);

    napi_set_named_property(env, exports, "socket", socket_fn);
    napi_set_named_property(env, exports, "bind", bind_fn);
    napi_set_named_property(env, exports, "connect", connect_fn);
    napi_set_named_property(env, exports, "close", close_fn);

    return exports;
}

} // anonymous namespace

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)