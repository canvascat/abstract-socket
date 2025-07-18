#if !defined(__linux__) && !defined(__APPLE__)
# error "Only Linux and macOS are supported"
#endif

#include <napi.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

namespace {

Napi::Value Socket(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
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

    return Napi::Number::New(env, fd);
}

Napi::Value Bind(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int32_t fd = info[0].As<Napi::Number>().Int32Value();
    std::string path = info[1].As<Napi::String>().Utf8Value();

    int err = 0;

    if (path[0] != '\0') {
        err = -EINVAL;
        goto out;
    }

    if (path.length() > sizeof(sockaddr_un::sun_path)) {
        err = -EINVAL;
        goto out;
    }

    sockaddr_un s;
    memset(&s, 0, sizeof(s));
    memcpy(s.sun_path, path.c_str(), path.length());
    s.sun_family = AF_UNIX;

    if (bind(fd, reinterpret_cast<sockaddr*>(&s), sizeof(s)) != 0) {
        err = -errno;
    }

out:
    return Napi::Number::New(env, err);
}

Napi::Value Connect(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int32_t fd = info[0].As<Napi::Number>().Int32Value();
    std::string path = info[1].As<Napi::String>().Utf8Value();

    int err = 0;

    if (path[0] != '\0') {
        err = -EINVAL;
        goto out;
    }

    if (path.length() > sizeof(sockaddr_un::sun_path)) {
        err = -EINVAL;
        goto out;
    }

    sockaddr_un s;
    memset(&s, 0, sizeof(s));
    memcpy(s.sun_path, path.c_str(), path.length());
    s.sun_family = AF_UNIX;

    if (connect(fd, reinterpret_cast<sockaddr*>(&s), sizeof(s)) != 0) {
        err = -errno;
    }

out:
    return Napi::Number::New(env, err);
}

Napi::Value Close(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int32_t fd = info[0].As<Napi::Number>().Int32Value();

    int err = 0;
    if (close(fd)) {
        err = -errno;
    }

    return Napi::Number::New(env, err);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("socket", Napi::Function::New(env, Socket));
    exports.Set("bind", Napi::Function::New(env, Bind));
    exports.Set("connect", Napi::Function::New(env, Connect));
    exports.Set("close", Napi::Function::New(env, Close));
    return exports;
}

} // anonymous namespace

NODE_API_MODULE(abstract_socket, Init)