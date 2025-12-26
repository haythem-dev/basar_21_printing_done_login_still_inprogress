# Design Guidelines: Package Analysis & Development Tool

## Design Approach
**Selected Framework**: Design System Approach (Carbon Design + GitHub-inspired patterns)
**Rationale**: Developer-focused utility application requiring efficient data visualization, code display, and file navigation. Prioritizes clarity, scannable information hierarchy, and professional aesthetics.

## Typography System

**Font Stack**:
- Primary: Inter (Google Fonts) - UI elements, body text
- Monospace: JetBrains Mono (Google Fonts) - code snippets, file paths, technical data

**Hierarchy**:
- Page Titles: text-3xl, font-semibold
- Section Headers: text-xl, font-semibold
- Card Titles: text-lg, font-medium
- Body Text: text-base, font-normal
- Code/Technical: text-sm, font-mono
- Metadata/Labels: text-xs, uppercase, tracking-wide, font-medium

## Layout System

**Spacing Primitives**: Use Tailwind units of 2, 4, 6, 8, 12, 16, 24
- Component padding: p-6 to p-8
- Section spacing: space-y-8 to space-y-12
- Grid gaps: gap-6
- Button padding: px-6 py-3

**Container Strategy**:
- Full application: max-w-screen-2xl mx-auto
- Content areas: max-w-6xl
- Code viewers: w-full (allow horizontal scroll if needed)

## Component Library

**Navigation**:
- Top navigation bar: Sticky header with logo, primary navigation links, user actions
- Sidebar navigation: Collapsible file tree/package explorer (w-64 expanded, w-16 collapsed)
- Breadcrumb trail: Show current location in package hierarchy

**Data Display Components**:
- Package cards: Horizontal layout with icon, name, version, description, metadata badges
- File tree: Nested list with expand/collapse, file type icons (from Heroicons)
- Dependency graph: Visual node-based relationship display
- Code viewer: Syntax-highlighted blocks with line numbers, copy button
- Stats dashboard: Grid of metric cards showing package size, dependencies, vulnerabilities
- Comparison table: Side-by-side package analysis

**Interactive Elements**:
- Search bar: Prominent, with autocomplete dropdown and advanced filters toggle
- Filter panels: Checkbox groups, radio selections, range sliders for package properties
- Tabs: Horizontal tabs for switching between Analysis/Dependencies/Code/Documentation views
- Action buttons: Primary CTAs use px-6 py-3 rounded-lg with subtle shadow
- Icon buttons: p-2 rounded hover states for quick actions

**Forms & Inputs**:
- Upload area: Drag-and-drop zone with dashed border, file icon, "Drop package here" text
- Text inputs: h-12 px-4 rounded-lg with focus states
- Select dropdowns: Custom styled with chevron icon

**Overlays**:
- Modal dialogs: Centered with backdrop blur, max-w-2xl
- Toast notifications: Top-right positioned, slide-in animation, auto-dismiss
- Loading states: Skeleton screens for data tables, spinner for quick actions

## Page Layouts

**Landing/Home Page**:
- Hero section: 70vh height with gradient background illustration (abstract code/network visualization)
- Upload CTA: Large drag-drop area as primary action
- Features grid: 3-column layout showcasing key capabilities (Package Analysis, Dependency Visualization, Security Scanning)
- Recent packages section: Horizontal scrolling card carousel
- Stats banner: 4-column metric display (Packages Analyzed, Dependencies Mapped, Vulnerabilities Found, Time Saved)

**Analysis Dashboard**:
- Two-column layout: Sidebar (package tree) + Main content area
- Header: Package name, version, action buttons (Download, Share, Compare)
- Tab navigation: Overview | Dependencies | Files | Security | Documentation
- Content area: Mixed layout with info cards, tables, and visualizations based on active tab

**Package Comparison View**:
- Split-screen layout: Two packages side-by-side
- Synchronized scrolling for aligned comparison
- Diff highlighting for differences in dependencies, versions, sizes

## Icons & Visual Elements

**Icon Library**: Heroicons (outline for navigation, solid for filled states)

**Key Icons**:
- Package: cube/box icon
- Files: document/folder icons with file type variations
- Dependencies: link/arrow-path icons  
- Security: shield/exclamation icons
- Search: magnifying-glass
- Upload: cloud-arrow-up

**Visual Accents**:
- Badges: Rounded-full px-3 py-1 for versions, tags, status indicators
- Dividers: Subtle horizontal rules to separate sections
- Shadows: Subtle on cards (shadow-sm), medium on modals (shadow-lg)

## Images

**Hero Section Image**: 
- Abstract, modern illustration of interconnected nodes/packages forming a network
- Gradient overlay (top-to-bottom fade) to ensure text readability
- Buttons on hero use backdrop-blur-md with semi-transparent backgrounds
- Image placement: Full-width background, contained within hero bounds

**Feature Section Graphics**:
- Icon illustrations for each feature card (package analysis visual, dependency tree diagram, security scan interface)
- Placement: Above or beside feature descriptions in 3-column grid

**Empty States**:
- Friendly illustration when no packages uploaded yet
- Simple line art style showing upload action

## Animations

Use sparingly and purposefully:
- Page transitions: Subtle fade (200ms)
- Hover states: Scale 1.02 on cards, underline on links
- Loading: Pulse animation on skeleton screens
- Tree expansion: Smooth height transition (150ms)

## Responsive Behavior

**Breakpoints**:
- Mobile (< 768px): Stack all columns, collapsible sidebar becomes bottom sheet
- Tablet (768px - 1024px): 2-column layouts, reduced spacing
- Desktop (> 1024px): Full multi-column layouts, expanded sidebar

**Mobile Adaptations**:
- Hamburger menu replaces horizontal navigation
- Tables become scrollable cards
- Side-by-side comparisons stack vertically
- Reduced padding (p-4 instead of p-8)

---

This design creates a professional, efficient interface optimized for developers analyzing packages, with clear information hierarchy and powerful data visualization capabilities.