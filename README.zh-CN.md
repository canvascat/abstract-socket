# better-abstractsocket

[English](README.md)  | [简体中文](README.zh-CN.md)

`better-abstractsocket` 是一个 Node.js 模块，用于创建和操作抽象 Unix 域套接字，仅支持 Linux 和 macOS 系统。该模块封装了底层的套接字操作，为开发者提供了更便捷的接口来创建和管理抽象套接字。

## 功能特性
- **平台支持**：仅支持 Linux 和 macOS 系统。
- **核心功能**：提供了创建套接字（`socket`）、绑定地址（`bind`）、连接套接字（`connect`）和关闭套接字（`close`）的功能。

## 安装
```bash
pnpm install better-abstractsocket
```

## 使用示例
### 创建服务器
```typescript
import AbstractSocket from 'better-abstractsocket';

const server = new AbstractSocket.Server();
server.listen('better-abstractsocket-example', () => {
    console.log('Server is listening on better-abstractsocket-example');
});
```

### 创建连接
```typescript
import { createConnection } from 'better-abstractsocket';

const socket = createConnection('better-abstractsocket-example', () => {
    console.log('Connected to abstract socket');
});
```

## API 文档
### `AbstractSocket.Server`
继承自 `net.Server`，提供 `listen` 方法用于监听抽象套接字。

### `createConnection`
用于创建一个到指定抽象套接字的连接。

### `supported`
一个布尔值，指示当前环境是否支持 `better-abstractsocket` 模块。

