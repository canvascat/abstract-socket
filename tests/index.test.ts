import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest'
import * as net from 'node:net'
import { AbstractSocket, AbstractSocketErrnoException } from '../src/index'

const testPath = 'test-abstract-socket'

// 清理测试文件的函数
function cleanup() {
  try {
    // 实际使用时需要实现具体的清理逻辑
    // 这里仅作示例
  } catch (error) {
    console.error('Cleanup error:', error)
  }
}

beforeEach(() => {
  cleanup()
})

afterEach(() => {
  cleanup()
})

describe('AbstractSocket', () => {
  it('should have supported property', () => {
    expect(AbstractSocket).toHaveProperty('supported')
    expect(typeof AbstractSocket.supported).toBe('boolean')
  })

  describe('Server', () => {
    it('should create a server instance', () => {
      if (AbstractSocket.supported) {
        const server = new AbstractSocket.Server()
        expect(server).toBeInstanceOf(AbstractSocket.Server)
        server.close()
      }
    })

    it('should emit error on invalid socket creation', () => {
      if (AbstractSocket.supported) {
        const server = new AbstractSocket.Server()
        const spy = vi.spyOn(server, 'emit')
        // 模拟无效的 socket 创建
        server.listen('invalid-path')
        expect(spy).toHaveBeenCalledWith('error', expect.any(AbstractSocketErrnoException))
        server.close()
      }
    })
  })

  describe('createConnection', () => {
    it('should create a connection', () => {
      if (AbstractSocket.supported) {
        const server = new AbstractSocket.Server()
        server.listen(testPath)

        const connectSpy = vi.fn()
        const socket = AbstractSocket.createConnection(testPath, connectSpy)

        expect(socket).toBeInstanceOf(net.Socket)

        server.close()
        socket.destroy()
      }
    })

    it('should emit error on invalid connection', () => {
      if (AbstractSocket.supported) {
        const spy = vi.fn()
        const socket = AbstractSocket.createConnection('invalid-path', spy)

        expect(spy).toHaveBeenCalled()
        socket.destroy()
      }
    })
  })
})
