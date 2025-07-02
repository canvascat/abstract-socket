import * as net from 'node:net'
import { createRequire } from 'node:module'

const require = createRequire(import.meta.url)


// https://github.com/saghul/node-abstractsocket

interface AbstractSocketBinding {
  socket: () => number
  bind: (fd: number, name: string) => number
  connect: (fd: number, name: string) => number
  close: (fd: number) => void
}

let binding: AbstractSocketBinding = {
  socket: () => -1,
  bind: () => -1,
  connect: () => -1,
  close: () => { },
}
let supported = false
try {
  binding = require('../build/Release/abstract-socket.node')
  supported = true;
} catch (error) {
  console.error('Failed to load abstract-socket bindings:', error)
}


export class AbstractSocketErrnoException extends Error implements NodeJS.ErrnoException {
  code: string
  errno: number
  syscall: string
  constructor(errno: number, syscall: string, extra?: any) {
    super(`Abstract socket error: ${syscall} failed with code ${errno}`)
    this.code = 'ERR_SOCKET'
    this.errno = errno
    this.syscall = syscall
    if (extra) {
      Object.assign(this, extra)
    }
  }
}

class AbstractSocketServer extends net.Server {
  // @ts-ignore
  listen(path: string, listeningListener?: () => void) {
    let err = binding.socket()
    if (err < 0) {
      this.emit('error', new AbstractSocketErrnoException(err, 'socket'))
    }

    const handle = { fd: err }

    err = binding.bind(err, path)
    if (err < 0) {
      binding.close(handle.fd)

      this.emit('error', new AbstractSocketErrnoException(err, 'bind'))
    }
    super.listen(handle, listeningListener)
  }
}

function createConnection(name: string, connectListener?: (socket: net.Socket) => void) {
  const defaultOptions = {
    readable: true,
    writable: true,
  }

  let err = binding.socket()
  if (err < 0) {
    const sock = new net.Socket(defaultOptions)
    setImmediate(() => sock.emit('error', new AbstractSocketErrnoException(err, 'socket')))
    connectListener?.(sock)
    return sock
  }

  const options = Object.assign({ fd: err }, defaultOptions)

  // yes, connect is synchronous, so sue me
  err = binding.connect(err, name)
  if (err < 0) {
    binding.close(options.fd)
    const sock = new net.Socket(defaultOptions)
    setImmediate(() => sock.emit('error', new AbstractSocketErrnoException(err, 'connect')))
    connectListener?.(sock)
    return sock
  }

  const sock = new net.Socket(options)
  connectListener?.(sock)
  return sock
};

export const AbstractSocket = {
  Server: AbstractSocketServer,
  createConnection,
  supported
}

export { AbstractSocketServer as Server, createConnection };

export default AbstractSocket
