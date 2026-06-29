<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Arcade Brick Game</title>
    <style>
        body { background: #000; color: #fff; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; margin: 0; font-family: 'Courier New', monospace; user-select: none; }
        #game { background: #000; border: 4px solid #fff; padding: 10px; border-radius: 4px; box-shadow: 0 0 20px rgba(255,255,255,0.2); }
        #hud { display: flex; justify-content: space-between; font-size: 14px; font-weight: bold; margin-bottom: 8px; color: #fff; border-bottom: 2px solid #fff; padding-bottom: 4px; }
        canvas { background: #000; display: block; }
    </style>
</head>
<body>
    <div id="game">
        <div id="hud"><span>SCORE:<span id="s">0</span></span><span>LIVES:<span id="l">3</span></span></div>
        <canvas id="c" width="240" height="200"></canvas>
    </div>
<script>
    const canvas = document.getElementById("c"), ctx = canvas.getContext("2d");
    let x, y, dx, dy, r = 3, pW = 40, pH = 6, pX;
    let rP = false, lP = false, rows = 6, cols = 5, bW = 40, bH = 10, pad = 4, topOff = 15, leftOff = 12;
    let score, lives, bricks, gameOver, gameWon;
    
    // Colores del arcoíris para las filas de bloques
    const rainbowColors = ["#ff0000", "#ff7f00", "#ffff00", "#00ff00", "#0000ff", "#4b0082"];

    function init() {
        x = 120; y = 160; dx = 1.8; dy = -1.8; pX = 100;
        score = 0; lives = 3; gameOver = false; gameWon = false;
        bricks = [];
        for(let c=0; c<cols; c++) {
            bricks[c] = [];
            for(let r=0; r<rows; r++) bricks[c][r] = { x:0, y:0, s:1 };
        }
        document.getElementById("s").innerText = "0";
        document.getElementById("l").innerText = "3";
    }

    document.addEventListener("keydown", e => {
        if(e.key=="Right"||e.key=="ArrowRight") rP=true;
        if(e.key=="Left"||e.key=="ArrowLeft") lP=true;
        if((e.key=="r"||e.key=="R") && (gameOver || gameWon)) init();
    });
    document.addEventListener("keyup", e => {
        if(e.key=="Right"||e.key=="ArrowRight") rP=false;
        if(e.key=="Left"||e.key=="ArrowLeft") lP=false;
    });

    function drawText(text1, text2) {
        ctx.fillStyle = "rgba(0, 0, 0, 0.9)";
        ctx.fillRect(10, 60, 220, 80);
        ctx.strokeStyle = "#fff";
        ctx.strokeRect(12, 62, 216, 76);
        ctx.fillStyle = "#fff";
        ctx.font = "bold 11px 'Courier New', monospace";
        ctx.textAlign = "center";
        ctx.fillText(text1, 120, 95);
        ctx.fillText(text2, 120, 115);
    }

    function draw() {
        ctx.clearRect(0, 0, 240, 200);
        
        // Procesar y dibujar bloques arcoíris
        for(let c=0; c<cols; c++) {
            for(let r=0; r<rows; r++) {
                if(bricks[c][r].s == 1) {
                    let bX = (c*(bW+pad))+leftOff, bY = (r*(bH+pad))+topOff;
                    bricks[c][r].x = bX; bricks[c][r].y = bY;
                    ctx.fillStyle = rainbowColors[r % rainbowColors.length];
                    ctx.fillRect(bX, bY, bW, bH);

                    if(!gameOver && !gameWon && x > bX && x < bX+bW && y > bY && y < bY+bH) {
                        dy = -dy; bricks[c][r].s = 0; score++;
                        document.getElementById("s").innerText = score*10;
                        if(score == rows*cols) gameWon = true;
                    }
                }
            }
        }

        // Dibujar paleta y pelota blancas
        ctx.fillStyle = "#fff";
        ctx.fillRect(pX, 200-pH-4, pW, pH);
        if (!gameOver && !gameWon) ctx.fillRect(x-r, y-r, r*2, r*2);

        // Lógica de pantallas de estado con textos solicitados
        if (gameOver) {
            drawText("HAS PERDIDO", "PULSA R PARA REINICIAR");
        } else if (gameWon) {
            drawText("HAS GANADO", "PULSA R VOLVER A JUGAR");
        } else {
            // Movimiento y colisiones
            if(x + dx > 240-r || x + dx < r) dx = -dx;
            if(y + dy < r) dy = -dy; 
            else if(y + dy > 200-r-4) { 
                if(x > pX && x < pX + pW) { dy = -dy; dx = 8 * ((x-(pX+pW/2))/pW); } 
                else { 
                    lives--; document.getElementById("l").innerText = lives; 
                    if(!lives) gameOver = true; 
                    else { x = 120; y = 160; dx = 1.8; dy = -1.8; pX = 100; } 
                }
            }
            if(rP && pX < 240-pW) pX += 5; else if(lP && pX > 0) pX -= 5;
            x += dx; y += dy;
        }
        requestAnimationFrame(draw);
    }

    init();
    draw();
</script>
</body>
</html>
