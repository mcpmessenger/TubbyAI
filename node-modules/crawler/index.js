// TubbyAI Playwright Crawler Module
// This module handles web crawling and interaction capture

const { chromium } = require('playwright');
const fs = require('fs');
const path = require('path');

class TubbyAICrawler {
  constructor(options = {}) {
    this.options = {
      headless: options.headless !== false,
      maxDepth: options.maxDepth || 3,
      outputDir: options.outputDir || './crawled_data',
      ...options
    };
    this.browser = null;
    this.context = null;
    this.page = null;
    this.crawledData = [];
  }

  async initialize() {
    console.log('Initializing TubbyAI Crawler...');
    
    // Create output directory
    if (!fs.existsSync(this.options.outputDir)) {
      fs.mkdirSync(this.options.outputDir, { recursive: true });
    }

    // Launch browser
    this.browser = await chromium.launch({
      headless: this.options.headless,
      args: ['--no-sandbox', '--disable-setuid-sandbox']
    });

    this.context = await this.browser.newContext({
      viewport: { width: 1920, height: 1080 },
      userAgent: 'TubbyAI-Crawler/1.0'
    });

    this.page = await this.context.newPage();
    console.log('Crawler initialized successfully');
  }

  async crawlWebsite(url) {
    console.log(`Starting crawl of: ${url}`);
    
    try {
      await this.page.goto(url, { waitUntil: 'networkidle' });
      
      // Find all interactive elements
      const interactiveElements = await this.findInteractiveElements();
      console.log(`Found ${interactiveElements.length} interactive elements`);

      // Process each element
      for (const element of interactiveElements) {
        await this.processElement(element, url);
      }

      return this.crawledData;
    } catch (error) {
      console.error(`Error crawling ${url}:`, error);
      throw error;
    }
  }

  async findInteractiveElements() {
    const selectors = [
      'button',
      'a[href]',
      'input[type="button"]',
      'input[type="submit"]',
      'input[type="checkbox"]',
      'input[type="radio"]',
      'select',
      '[onclick]',
      '[role="button"]',
      '[role="link"]',
      '[role="menuitem"]'
    ];

    const elements = [];
    
    for (const selector of selectors) {
      const found = await this.page.$$(selector);
      for (const element of found) {
        try {
          const isVisible = await element.isVisible();
          if (isVisible) {
            const text = await element.textContent();
            const tagName = await element.evaluate(el => el.tagName);
            const id = await element.getAttribute('id');
            const className = await element.getAttribute('class');
            
            elements.push({
              element,
              selector: this.generateSelector(element),
              text: text?.trim() || '',
              tagName: tagName?.toLowerCase(),
              id,
              className,
              boundingBox: await element.boundingBox()
            });
          }
        } catch (error) {
          // Skip elements that can't be processed
          continue;
        }
      }
    }

    return elements;
  }

  async generateSelector(element) {
    try {
      return await element.evaluate(el => {
        if (el.id) return `#${el.id}`;
        if (el.className) return `.${el.className.split(' ')[0]}`;
        return el.tagName.toLowerCase();
      });
    } catch {
      return 'unknown';
    }
  }

  async processElement(elementInfo, baseUrl) {
    const { element, selector, text, tagName } = elementInfo;
    
    try {
      console.log(`Processing element: ${selector} (${text})`);

      // Take pre-interaction screenshot
      const preScreenshot = await this.takeElementScreenshot(element, 'pre');

      // Simulate interaction
      await this.simulateInteraction(element, tagName);

      // Wait for potential changes
      await this.page.waitForTimeout(1000);

      // Take post-interaction screenshot
      const postScreenshot = await this.takeElementScreenshot(element, 'post');

      // Create interaction data
      const interactionData = {
        website_url: baseUrl,
        element_selector: selector,
        element_text: text,
        page_url: this.page.url(),
        pre_interaction_thumbnail: preScreenshot,
        post_interaction_thumbnail: postScreenshot,
        interaction_gif_or_video: null, // TODO: Create GIF
        timestamp: new Date().toISOString()
      };

      this.crawledData.push(interactionData);
      console.log(`Captured interaction for: ${selector}`);

    } catch (error) {
      console.error(`Error processing element ${selector}:`, error);
    }
  }

  async takeElementScreenshot(element, prefix) {
    try {
      const boundingBox = await element.boundingBox();
      if (!boundingBox) return null;

      const filename = `${prefix}_${Date.now()}_${Math.random().toString(36).substr(2, 9)}.png`;
      const filepath = path.join(this.options.outputDir, filename);

      await element.screenshot({
        path: filepath,
        type: 'png'
      });

      return filepath;
    } catch (error) {
      console.error(`Error taking screenshot:`, error);
      return null;
    }
  }

  async simulateInteraction(element, tagName) {
    try {
      switch (tagName) {
        case 'button':
        case 'input':
          await element.click();
          break;
        case 'a':
          // For links, we might want to just hover
          await element.hover();
          break;
        case 'select':
          await element.selectOption({ index: 1 });
          break;
        case 'input[type="checkbox"]':
        case 'input[type="radio"]':
          await element.check();
          break;
        default:
          await element.hover();
      }
    } catch (error) {
      console.error(`Error simulating interaction:`, error);
    }
  }

  async close() {
    if (this.browser) {
      await this.browser.close();
    }
  }

  getCrawledData() {
    return this.crawledData;
  }
}

module.exports = TubbyAICrawler;
