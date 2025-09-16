# TubbyAI Browser

A Chromium-based browser with an innovative "Tooltip Companion" that provides interactive previews of web elements.

## Features

- **Tooltip Companion**: Hover over interactive elements to see visual previews of what happens when you click them
- **Smart Crawling**: Automated website crawling using Playwright to capture interaction data
- **Local Storage**: All interaction data stored locally for privacy and performance
- **Seamless Integration**: Built on Chromium for familiar browsing experience

## Development Setup

### Prerequisites

- Windows 10/11
- Git
- Python 3.11+
- Node.js 20+
- Visual Studio 2022 (for Chromium compilation)

### Quick Start

1. Clone the repository:
```bash
git clone https://github.com/mcpmessenger/TubbyAI.git
cd TubbyAI
```

2. Run the setup script:
```powershell
.\setup_chromium.ps1
```

3. Build Chromium:
```bash
cd chromium
gclient runhooks
gn gen out/Default
autoninja -C out/Default chrome
```

## Architecture

- **Chromium Fork**: Base browser functionality
- **Tooltip Module**: UI integration for displaying previews
- **Crawler Module**: Playwright-based web crawling
- **Data Storage**: Local database for interaction data

## Project Status

🚧 **In Development** - Currently setting up Chromium fork and core modules

## Contributing

This project is in active development. Check the issues tab for current tasks and milestones.

## License

TBD - Will be determined based on Chromium licensing requirements
