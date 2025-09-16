# TubbyAI Chromium Setup Script
# This script sets up the development environment for TubbyAI Browser

Write-Host "Setting up TubbyAI Chromium development environment..." -ForegroundColor Green

# Add depot_tools to PATH
$depotToolsPath = Join-Path $PSScriptRoot "depot_tools"
$env:PATH = "$depotToolsPath;$env:PATH"

Write-Host "Added depot_tools to PATH" -ForegroundColor Yellow

# Verify depot_tools installation
try {
    gclient --version
    Write-Host "depot_tools verified successfully" -ForegroundColor Green
} catch {
    Write-Host "Error: depot_tools not found. Please check the installation." -ForegroundColor Red
    exit 1
}

# Create chromium directory
$chromiumDir = Join-Path $PSScriptRoot "chromium"
if (!(Test-Path $chromiumDir)) {
    New-Item -ItemType Directory -Path $chromiumDir
    Write-Host "Created chromium directory" -ForegroundColor Yellow
}

# Navigate to chromium directory
Set-Location $chromiumDir

Write-Host "Starting Chromium source fetch..." -ForegroundColor Green
Write-Host "This will take a while (several GB download)..." -ForegroundColor Yellow

# Fetch Chromium source
try {
    fetch --nohooks chromium
    Write-Host "Chromium source fetched successfully!" -ForegroundColor Green
} catch {
    Write-Host "Error fetching Chromium source: $_" -ForegroundColor Red
    exit 1
}

Write-Host "Setup complete! Next steps:" -ForegroundColor Green
Write-Host "1. cd chromium" -ForegroundColor Yellow
Write-Host "2. gclient runhooks" -ForegroundColor Yellow
Write-Host "3. gn gen out/Default" -ForegroundColor Yellow
Write-Host "4. autoninja -C out/Default chrome" -ForegroundColor Yellow
