# abstract-socket

`abstract-socket` 是一个 Node.js 模块，用于创建和操作抽象 Unix 域套接字，仅支持 Linux 和 macOS 系统。该模块封装了底层的套接字操作，为开发者提供了更便捷的接口来创建和管理抽象套接字。

## 功能特性
- **平台支持**：仅支持 Linux 和 macOS 系统。
- **核心功能**：提供了创建套接字（`socket`）、绑定地址（`bind`）、连接套接字（`connect`）和关闭套接字（`close`）的功能。
- **异常处理**：自定义 `AbstractSocketErrnoException` 类，方便处理套接字操作中的错误。

## 安装
```bash
pnpm install abstract-socket
```

## 使用示例
### 创建服务器
```typescript
import AbstractSocket from 'abstract-socket';

const server = new AbstractSocket.Server();
server.listen('abstract-socket-example', () => {
    console.log('Server is listening on abstract-socket-example');
});
```

### 创建连接
```typescript
import { createConnection } from 'abstract-socket';

const socket = createConnection('abstract-socket-example', () => {
    console.log('Connected to abstract socket');
});
```

## API 文档
### `AbstractSocket.Server`
继承自 `net.Server`，提供 `listen` 方法用于监听抽象套接字。

### `createConnection`
用于创建一个到指定抽象套接字的连接。

### `supported`
一个布尔值，指示当前环境是否支持 `abstract-socket` 模块。

## 开发依赖
- `@types/node`: `^24.0.10`
- `tsdown`: `^0.12.9`
- `typescript`: `^5.8.3`
- `vitest`: `^3.2.4`

## 生产依赖
- `nan`: `^2.22.2`
- `prebuild-install`: `^7.1.3`

## 构建和安装
```bash
# 构建项目
pnpm run build

# 安装并构建发布版本（仅 Linux 和 macOS）
pnpm run install
```

## 许可协议
本项目采用 [MIT 许可协议](LICENSE)。
