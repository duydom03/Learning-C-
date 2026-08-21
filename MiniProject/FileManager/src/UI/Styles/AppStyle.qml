pragma Singleton
import QtQuick 2.15

QtObject {
    // ── Background & Surface ──────────────────────────────────────────
    readonly property color backgroundColor : "#F8F9FA"
    readonly property color cardBackground  : "#FFFFFF"
    readonly property color hoverColor      : "#F1F3F4"
    readonly property color selectedColor   : "#E8F0FE"
    readonly property color sidebarBg       : "#FFFFFF"

    // ── Primary (Google Blue) ─────────────────────────────────────────
    readonly property color primaryColor    : "#4285F4"
    readonly property color primaryDark     : "#1A73E8"
    readonly property color primaryLight    : "#D2E3FC"
    readonly property color primaryFaint    : "#E8F0FE"

    // ── Text ──────────────────────────────────────────────────────────
    readonly property color textPrimary     : "#202124"
    readonly property color textSecondary   : "#5F6368"
    readonly property color textDisabled    : "#9AA0A6"
    readonly property color textOnPrimary   : "#FFFFFF"

    // ── Borders & Dividers ───────────────────────────────────────────
    readonly property color borderColor     : "#E8EAED"
    readonly property color dividerColor    : "#F1F3F4"

    // ── Status Colors ─────────────────────────────────────────────────
    readonly property color errorColor      : "#D93025"
    readonly property color errorFaint      : "#FCE8E6"
    readonly property color successColor    : "#34A853"
    readonly property color successFaint    : "#E6F4EA"
    readonly property color warningColor    : "#F9AB00"
    readonly property color warningFaint    : "#FEF7E0"

    // ── File Type Colors ──────────────────────────────────────────────
    readonly property color colorFolder     : "#F9AB00"
    readonly property color colorImage      : "#EA4335"
    readonly property color colorDocument   : "#4285F4"
    readonly property color colorSheet      : "#34A853"
    readonly property color colorPdf        : "#D93025"
    readonly property color colorVideo      : "#9334EA"
    readonly property color colorArchive    : "#8D6E63"
    readonly property color colorCode       : "#FF6D00"
    readonly property color colorAudio      : "#00A8E0"

    // ── Typography ────────────────────────────────────────────────────
    readonly property string fontFamily     : "Segoe UI"
    readonly property string fontFamilyMono : "Consolas"
    readonly property int    fontSizeXS     : 10
    readonly property int    fontSizeSM     : 12
    readonly property int    fontSizeMD     : 14
    readonly property int    fontSizeLG     : 16
    readonly property int    fontSizeXL     : 20
    readonly property int    fontSizeXXL    : 24

    // ── Spacing & Radius ──────────────────────────────────────────────
    readonly property int    radiusSM       : 6
    readonly property int    radiusMD       : 10
    readonly property int    radiusLG       : 16
    readonly property int    radiusXL       : 20
    readonly property int    radiusFull     : 999

    readonly property int    spacingXS      : 4
    readonly property int    spacingSM      : 8
    readonly property int    spacingMD      : 12
    readonly property int    spacingLG      : 16
    readonly property int    spacingXL      : 24

    // ── Elevation (Shadow) ────────────────────────────────────────────
    readonly property int    elevationCard  : 2
    readonly property int    elevationPopup : 8
    readonly property int    elevationModal : 16

    // ── Animation ─────────────────────────────────────────────────────
    readonly property int    durationFast   : 120
    readonly property int    durationMid    : 200
    readonly property int    durationSlow   : 320
    readonly property int    easingType     : Easing.OutCubic

    // ── Toolbar / Nav heights ─────────────────────────────────────────
    readonly property int    appBarHeight   : 56
    readonly property int    toolbarHeight  : 56
    readonly property int    navPaneWidth   : 220
    readonly property int    navPaneMin     : 160
    readonly property int    navPaneMax     : 360

    // ── Helpers ───────────────────────────────────────────────────────
    function fileTypeColor(iconType) {
        switch (iconType) {
        case "folder":      return colorFolder
        case "image":       return colorImage
        case "document":    return colorDocument
        case "spreadsheet": return colorSheet
        case "pdf":         return colorPdf
        case "video":       return colorVideo
        case "archive":     return colorArchive
        case "code":        return colorCode
        case "audio":       return colorAudio
        default:            return textDisabled
        }
    }
}
