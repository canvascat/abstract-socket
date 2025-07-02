# better-abstractsocket

[English](README.md)  | [简体中文](README.zh-CN.md)

`better-abstractsocket` is a Node.js module designed to create and manipulate abstract Unix domain sockets. It only supports Linux and macOS systems. This module encapsulates low - level socket operations, providing developers with a more convenient interface to create and manage abstract sockets.

## Features
- **Platform Support**: Only supports Linux and macOS systems.
- **Core Functions**: Provides functions to create sockets (`socket`), bind addresses (`bind`), connect to sockets (`connect`), and close sockets (`close`).

## Installation
```bash
pnpm install better-abstractsocket
```

## Usage Examples
### Create a Server
```typescript
import AbstractSocket from 'better-abstractsocket';

const server = new AbstractSocket.Server();
server.listen('better-abstractsocket-example', () => {
    console.log('Server is listening on better-abstractsocket-example');
});
```

### Create a Connection
```typescript
import { createConnection } from 'better-abstractsocket';

const socket = createConnection('better-abstractsocket-example', () => {
    console.log('Connected to abstract socket');
});
```

## API Documentation
### `AbstractSocket.Server`
Inherits from `net.Server` and provides the `listen` method to listen on an abstract socket.

### `createConnection`
Used to create a connection to a specified abstract socket.

### `supported`
A boolean value indicating whether the current environment supports the `better-abstractsocket` module.
