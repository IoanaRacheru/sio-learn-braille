param(
    [ValidateSet(0, 1)]
    [int]$EnableDfplayerTest = 1,
    [string]$ArduinoCli = '',
    [string]$BuildPath = ''
)

$ErrorActionPreference = 'Stop'
if (-not $ArduinoCli) {
    $cliCommand = Get-Command arduino-cli -ErrorAction SilentlyContinue
    if ($cliCommand) {
        $ArduinoCli = $cliCommand.Source
    } else {
        $ArduinoCli = Join-Path $env:LOCALAPPDATA 'Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe'
    }
}
if (-not (Test-Path -LiteralPath $ArduinoCli -PathType Leaf)) {
    throw 'Arduino CLI not found. Pass -ArduinoCli with the executable path.'
}

# A compiler property applies to the sketch, all .cpp modules, and libraries.
$compileArgs = @(
    'compile', '--fqbn', 'arduino:avr:uno',
    '--build-property', "compiler.cpp.extra_flags=-DENABLE_DFPLAYER_TEST=$EnableDfplayerTest"
)
if ($BuildPath) { $compileArgs += @('--build-path', $BuildPath) }
$compileArgs += $PSScriptRoot
& $ArduinoCli @compileArgs
exit $LASTEXITCODE
