if (['linux', 'darwin'].includes(process.platform)) {
  if (!require('fs').existsSync('./binding.gyp')) {
    require('fs').copyFileSync('./_binding.gyp', './binding.gyp')
  }
  require('child_process').execSync('prebuild-install || node-gyp rebuild --release', { stdio: 'inherit' });
} else {
  console.debug('Skipping node-gyp rebuild on ' + process.platform);
  if (require('fs').existsSync('./binding.gyp')) {
    require('fs').rmSync('./binding.gyp', { recursive: true, force: true })
  }
}
