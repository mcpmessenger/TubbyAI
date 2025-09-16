# Chromium Download Monitor
# This script monitors the Chromium download progress

param(
    [int]$RefreshInterval = 30  # Refresh every 30 seconds
)

$chromiumPath = "C:\Users\senti\OneDrive\Desktop\TubbyAI\TubbyAI-Browser\chromium"
$startTime = Get-Date

Write-Host "🔍 Monitoring Chromium Download Progress" -ForegroundColor Green
Write-Host "📁 Path: $chromiumPath" -ForegroundColor Yellow
Write-Host "⏰ Started: $startTime" -ForegroundColor Yellow
Write-Host "🔄 Refresh Interval: $RefreshInterval seconds" -ForegroundColor Yellow
Write-Host "Press Ctrl+C to stop monitoring" -ForegroundColor Cyan
Write-Host ""

while ($true) {
    $currentTime = Get-Date
    $elapsed = $currentTime - $startTime
    
    Clear-Host
    Write-Host "🔍 Chromium Download Monitor" -ForegroundColor Green
    Write-Host "⏰ Elapsed Time: $($elapsed.ToString('hh\:mm\:ss'))" -ForegroundColor Yellow
    Write-Host "📁 Monitoring: $chromiumPath" -ForegroundColor Yellow
    Write-Host ""
    
    if (Test-Path $chromiumPath) {
        # Get directory size
        try {
            $size = (Get-ChildItem -Path $chromiumPath -Recurse -File | Measure-Object -Property Length -Sum).Sum
            $sizeGB = [math]::Round($size / 1GB, 2)
            $sizeMB = [math]::Round($size / 1MB, 2)
            
            Write-Host "📊 Current Size: $sizeGB GB ($sizeMB MB)" -ForegroundColor Green
            
            # Count files
            $fileCount = (Get-ChildItem -Path $chromiumPath -Recurse -File).Count
            Write-Host "📄 Files Downloaded: $fileCount" -ForegroundColor Green
            
            # Check for specific Chromium directories
            $srcPath = Join-Path $chromiumPath "src"
            if (Test-Path $srcPath) {
                Write-Host "✅ src/ directory found" -ForegroundColor Green
                
                # Check for key Chromium files
                $chromePath = Join-Path $srcPath "chrome"
                $contentPath = Join-Path $srcPath "content"
                $thirdPartyPath = Join-Path $srcPath "third_party"
                
                if (Test-Path $chromePath) { Write-Host "✅ chrome/ directory found" -ForegroundColor Green }
                if (Test-Path $contentPath) { Write-Host "✅ content/ directory found" -ForegroundColor Green }
                if (Test-Path $thirdPartyPath) { Write-Host "✅ third_party/ directory found" -ForegroundColor Green }
            } else {
                Write-Host "⏳ src/ directory not yet created..." -ForegroundColor Yellow
            }
            
        } catch {
            Write-Host "❌ Error reading directory: $($_.Exception.Message)" -ForegroundColor Red
        }
    } else {
        Write-Host "❌ Chromium directory not found!" -ForegroundColor Red
    }
    
    Write-Host ""
    Write-Host "🔄 Next check in $RefreshInterval seconds..." -ForegroundColor Cyan
    Write-Host "Press Ctrl+C to stop monitoring" -ForegroundColor Gray
    
    Start-Sleep -Seconds $RefreshInterval
}
