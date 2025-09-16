# Quick Chromium Status Checker
# Run this to get a quick snapshot of the download status

$chromiumPath = "C:\Users\senti\OneDrive\Desktop\TubbyAI\TubbyAI-Browser\chromium"

Write-Host "Chromium Download Status Check" -ForegroundColor Green
Write-Host "Path: $chromiumPath" -ForegroundColor Yellow
Write-Host ""

if (Test-Path $chromiumPath) {
    try {
        # Get directory size
        $size = (Get-ChildItem -Path $chromiumPath -Recurse -File | Measure-Object -Property Length -Sum).Sum
        $sizeGB = [math]::Round($size / 1GB, 2)
        $sizeMB = [math]::Round($size / 1MB, 2)
        
        Write-Host "Current Size: $sizeGB GB ($sizeMB MB)" -ForegroundColor Green
        
        # Count files
        $fileCount = (Get-ChildItem -Path $chromiumPath -Recurse -File).Count
        Write-Host "Files Downloaded: $fileCount" -ForegroundColor Green
        
        # Check for key directories
        $srcPath = Join-Path $chromiumPath "src"
        if (Test-Path $srcPath) {
            Write-Host "src/ directory exists" -ForegroundColor Green
            
            # Check for key Chromium directories
            $chromePath = Join-Path $srcPath "chrome"
            $contentPath = Join-Path $srcPath "content"
            $thirdPartyPath = Join-Path $srcPath "third_party"
            
            if (Test-Path $chromePath) { Write-Host "chrome/ directory exists" -ForegroundColor Green }
            if (Test-Path $contentPath) { Write-Host "content/ directory exists" -ForegroundColor Green }
            if (Test-Path $thirdPartyPath) { Write-Host "third_party/ directory exists" -ForegroundColor Green }
            
            # Check if download is complete (look for .gclient file)
            $gclientPath = Join-Path $chromiumPath ".gclient"
            if (Test-Path $gclientPath) {
                Write-Host ".gclient file found - download may be complete!" -ForegroundColor Green
            }
        } else {
            Write-Host "src/ directory not yet created - download in progress..." -ForegroundColor Yellow
        }
        
    } catch {
        Write-Host "Error reading directory: $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "Chromium directory not found!" -ForegroundColor Red
}

Write-Host ""
Write-Host "To monitor continuously, run: .\monitor_chromium.ps1" -ForegroundColor Cyan
