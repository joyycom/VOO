function selectionContains(selection, trackIndex, clipIndex) {
    if (selection[0] === trackIndex && selection[1] === clipIndex
            && !selection[2]) {
        return true
    }
    return false
}
function transitionSelectionContains(selection, trackIndex, clipIndex) {
    if (selection[0] === trackIndex && selection[1] === clipIndex
            && selection[2]) {
        return true
    }
    return false
}
