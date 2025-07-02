import { defineConfig } from 'tsdown';

export default defineConfig({
  target: ['node16'],
  entry: [
    'src/index.ts'
  ],
  dts: {
    sourcemap: true,
    tsconfig: './tsconfig.build.json',
  },
  format: ['cjs', 'esm'],
  outExtensions: (ctx) => ({
    dts: ctx.format === 'cjs' ? '.d.cts' : '.d.mts',
    js: ctx.format === 'cjs' ? '.cjs' : '.mjs',
  })
});
