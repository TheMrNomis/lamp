var canvasParams = {
    h: 1000,
    w: 2000
};

/**
 * An IlluminationPoint is a point on the illumination curve.
 * It both represents a point on the drawn canvas (via x and y getters)
 * and a point on the "illumination curve space" (between 0 and 1, with u and v getters)
 */
class IlluminationPoint {
    _u = 0;
    _v = 0;

    static fromCanvas(x, y) {
        var ret = new IlluminationPoint();

        ret._u = 1 - (x/canvasParams.h);
        ret._v = y/canvasParams.w;

        return ret;
    }

    static fromIllumination(u, v) {
        var ret = new IlluminationPoint();

        ret._u = u;
        ret._v = v;

        return ret;
    }

    get x() {
        return (1-this._u) * canvasParams.w;
    }

    get y() {
        return this._v * canvasParams.h;
    }

    get u() {
        return this._u;
    }

    get v() {
        return this._v;
    }
}

class Drawer {
    constructor(context) {
        this.draw = context;
        this.draw.lineWidth = 10;
    }

    bezier(p1, c1, c2, p2) {
        this.draw.strokeStyle = "black";
        this.draw.beginPath();
        this.draw.moveTo(p1.x, p1.y);
        this.draw.bezierCurveTo(c1.x, c1.y, c2.x, c2.y, p2.x, p2.y);
        this.draw.stroke();
    }

    controlHandle(p, c) {
        //line between p and c
        this.draw.strokeStyle = "grey";

        this.draw.moveTo(p.x, p.y);
        this.draw.lineTo(c.x, c.y);
        this.draw.stroke();
    }
}

window.addEventListener("load", function() {
    var canvas = document.getElementById("illuminationCurve");
    canvasParams.h = canvas.height;
    canvasParams.w = canvas.width;

    var context = canvas.getContext("2d");
    var draw = new Drawer(context);

    var p1 = IlluminationPoint.fromIllumination(0,0);
    var p2 = IlluminationPoint.fromIllumination(1,1);

    var c1 = IlluminationPoint.fromIllumination(0.2, 0);
    var c2 = IlluminationPoint.fromIllumination(0.8, 1);

    draw.bezier(p1, c1, c2, p2);
    draw.controlHandle(p1, c1);
    draw.controlHandle(p2, c2);
});
