var canvasParams = {
    h: 1000,
    w: 2000,
    lineWidth : 10,
    handleRadius : 40
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

        ret._u = x/canvasParams.w;
        ret._v = 1 - (y/canvasParams.h);

        return ret;
    }

    static fromIllumination(u, v) {
        var ret = new IlluminationPoint();

        ret._u = u;
        ret._v = v;

        return ret;
    }

    static distCanvas(p1, p2) {
        const x = p1.x - p2.x;
        const y = p1.y - p2.y;

        return Math.sqrt(x*x + y*y);
    }

    get x() {
        return this._u * canvasParams.w;
    }

    get y() {
        return (1-this._v) * canvasParams.h;
    }

    get u() {
        return this._u;
    }

    get v() {
        return this._v;
    }
}

/**
 * drawing functions for the bezier path and handles
 */
class Drawer {
    constructor(context) {
        this.draw = context;
        this.draw.lineWidth = canvasParams.lineWidth;
    }

    bezier(p1, c1, c2, p2) {
        this.draw.strokeStyle = "black";

        this.draw.beginPath();
        this.draw.moveTo(p1.x, p1.y);
        this.draw.bezierCurveTo(c1.x, c1.y, c2.x, c2.y, p2.x, p2.y);
        this.draw.stroke();
    }

    line(p1, p2) {
        this.draw.beginPath();
        this.draw.moveTo(p1.x, p1.y);
        this.draw.lineTo(p2.x, p2.y);
        this.draw.stroke();
    }

    circle(c, r) {
        this.draw.beginPath();
        this.draw.arc(c.x, c.y, r, 0, 2*Math.PI);
        this.draw.fill();
    }

    controlHandle(p, c) {
        //line between p and c
        this.draw.strokeStyle = "grey";

        this.line(p, c);
        this.circle(c, canvasParams.handleRadius);
    }

    drawCurve(p1, c1, c2, p2) {
        this.draw.clearRect(0, 0, canvasParams.w, canvasParams.h);

        this.bezier(p1, c1, c2, p2);
        this.controlHandle(p1, c1);
        this.controlHandle(p2, c2);
    }
}


/**
 * enumeration to encode if the user is moving a handle
 */
const ControlPointMoving = Object.freeze({
    none: 0,
    c1: 1,
    c2: 2
});

//main script below
window.addEventListener("load", function() {
    var canvas = document.getElementById("illuminationCurve");
    canvasParams.h = canvas.height;
    canvasParams.w = canvas.width;

    var context = canvas.getContext("2d");
    var drawer = new Drawer(context);

    var p1 = IlluminationPoint.fromIllumination(0,0);
    var p2 = IlluminationPoint.fromIllumination(1,1);

    var c1 = IlluminationPoint.fromIllumination(0.2, 0);
    var c2 = IlluminationPoint.fromIllumination(0.8, 1);

    function getCursorPosition(e) {
        const bbox = canvas.getBoundingClientRect();
        const u = (e.clientX - bbox.left) / bbox.width;
        const v = 1 - ((e.clientY - bbox.top) / bbox.height);
        return IlluminationPoint.fromIllumination(u, v);
    }

    drawer.drawCurve(p1, c1, c2, p2);

    //interaction
    var currentlyMoving = ControlPointMoving.none;
    canvas.addEventListener("mousedown", function(e) {
        const cursor = getCursorPosition(e);

        const maxDist = canvasParams.handleRadius;
        const distC1 = IlluminationPoint.distCanvas(cursor, c1);
        const distC2 = IlluminationPoint.distCanvas(cursor, c2);

        if(distC1 <= maxDist) {currentlyMoving = ControlPointMoving.c1;}
        if(distC2 <= maxDist) {currentlyMoving = ControlPointMoving.c2;}
    });

    canvas.addEventListener("mouseup", function(e) {
        currentlyMoving = ControlPointMoving.none;
    });

    canvas.addEventListener("mousemove", function(e) {
        if(currentlyMoving == ControlPointMoving.c1) {c1 = getCursorPosition(e);}
        if(currentlyMoving == ControlPointMoving.c2) {c2 = getCursorPosition(e);}

        if(currentlyMoving != ControlPointMoving.none) {drawer.drawCurve(p1, c1, c2, p2);}
    });
});
