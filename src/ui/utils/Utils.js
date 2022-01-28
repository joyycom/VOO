function calcAngle(p1, p2) {
    var xDistance = (p2.x - p1.x)
    var yDistance = (p2.y - p1.y)
    var xyDistance = Math.sqrt(xDistance * xDistance + yDistance * yDistance)
    var angle_sin = (Math.asin(yDistance / xyDistance) * 180 / Math.PI)

    return xDistance < 0 ? 90 - angle_sin : 270 + angle_sin
}


function calcAngle2(p1, p2) {
    var xDistance = (p2.x - p1.x)
    var yDistance = (p2.y - p1.y)
    var xyDistance = Math.sqrt(xDistance * xDistance + yDistance * yDistance)
    var angle_sin = (Math.asin(yDistance / xyDistance) * 180 / Math.PI)

    if(xDistance < 0){
        return 180 + angle_sin
    }else{
        return (360 - angle_sin)%360
    }
}
