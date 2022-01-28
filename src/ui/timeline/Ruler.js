function getScaleIndexFromMinMax(min, max, scale) {
    if (max - min <= 1) {
        return min
    }
    var mid = Math.floor(min + (max - min) / 2)
    var midScale = getTimeScale(mid, false)
    if (midScale > scale) {
        return getScaleIndexFromMinMax(mid, max, scale)
    } else {
        return getScaleIndexFromMinMax(min, mid, scale)
    }
}

function getTimeScale(_scaleIndex, notify) {
    var _numFrame, _allFrame, _rulerWidth
    if (_scaleIndex < 7) {
        _rulerWidth = 28 - _scaleIndex * 29 / 23
        _numFrame = 2
        _allFrame = 2
    } else if (_scaleIndex < 24) {
        _rulerWidth = 50 - _scaleIndex * 29 / 23
        if (_scaleIndex < 13) {
            _numFrame = 3
            _allFrame = 3
        } else if (_scaleIndex < 19) {
            _numFrame = 5
            _allFrame = 5
        } else {
            _numFrame = 10
            _allFrame = 10
        }
    } else if (_scaleIndex < 36) {
        _rulerWidth = 60 - (_scaleIndex - 24) * 43 / 11
        if (_scaleIndex < 31) {
            _numFrame = 5
            _allFrame = 15
        } else {
            _numFrame = 10
            _allFrame = 30
        }
    } else {
        _rulerWidth = 30 - (_scaleIndex - 36) % 6 * 9 / 5
        _numFrame = 10
        if (_scaleIndex < 36 + 6 * 1) {
            _allFrame = 60
        } else if (_scaleIndex < 36 + 6 * 2) {
            _allFrame = 90
        } else if (_scaleIndex < 36 + 6 * 3) {
            _allFrame = 150
        } else if (_scaleIndex < 36 + 6 * 4) {
            _allFrame = 300
        } else if (_scaleIndex < 36 + 6 * 5) {
            _allFrame = 900
        } else if (_scaleIndex < 36 + 6 * 6) {
            _allFrame = 1800
        } else if (_scaleIndex < 36 + 6 * 7) {
            _allFrame = 3600
        } else if (_scaleIndex < 36 + 6 * 8) {
            _allFrame = 5400
        } else if (_scaleIndex < 36 + 6 * 9) {
            _allFrame = 9000
        } else if (_scaleIndex < 36 + 6 * 10) {
            _allFrame = 18000
        } else if (_scaleIndex < 36 + 6 * 11) {
            _allFrame = 54000
        } else {
            _allFrame = 216000
        }
    }
    if (notify) {
        scaleIndex = _scaleIndex
        numFrame = _numFrame
        allFrame = _allFrame
        rulerWidth = _rulerWidth
    }
    return _rulerWidth * _numFrame / _allFrame
}

function setTimeScale(scale) {
    if (scale > 0) {
        if (scaleIndex >= bestMaxScaleIndex) {
            return
        }
        scaleIndex++
    } else {
        if (scaleIndex <= 0) {
            return
        }
        scaleIndex--
    }
    timelineController.timeScale = getTimeScale(scaleIndex, true)
    console.log("scaleIndex:" + scaleIndex + ",timeScale:" + timelineController.timeScale
                + ",rulerWidth:" + rulerWidth + ",numFrame:" + numFrame + ",allFrame:" + allFrame)
}

function zoomIn() {
    var zoomScaleIndex = bestMaxScaleIndex
    if (scaleIndex <= 7) {
        zoomScaleIndex = 0
    } else if (scaleIndex <= 13) {
        zoomScaleIndex = 7
    } else if (scaleIndex <= 19) {
        zoomScaleIndex = 13
    } else if (scaleIndex <= 36) {
        zoomScaleIndex = 19
    } else {
        for (var i = 1; i < 13; i++) {
            if (scaleIndex <= 36 + 6 * i) {
                zoomScaleIndex = 36 + 6 * (i - 1)
                break
            }
        }
    }
    return zoomScaleIndex
}

function zoomOut() {
    var zoomScaleIndex = bestMaxScaleIndex
    if (scaleIndex < 7) {
        zoomScaleIndex = 7
    } else if (scaleIndex < 13) {
        zoomScaleIndex = 13
    } else if (scaleIndex < 19) {
        zoomScaleIndex = 19
    } else {
        for (var i = 0; i < 13; i++) {
            if (scaleIndex < 36 + 6 * i) {
                zoomScaleIndex = 36 + 6 * i
                break
            }
        }
    }
    if (zoomScaleIndex > bestMaxScaleIndex) {
        return bestMaxScaleIndex
    }
    return zoomScaleIndex
}
