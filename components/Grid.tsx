
import React, { useRef, useEffect } from 'react';
import { GRID_WIDTH, GRID_HEIGHT, CELL_SIZE, GRID_LINE_COLOR } from '../constants';

interface GridProps {
  grid: number[][];
  aliveColor: string;
  deadColor: string;
  showGridLines: boolean;
}

const Grid: React.FC<GridProps> = ({ grid, aliveColor, deadColor, showGridLines }) => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    ctx.fillStyle = deadColor;
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    ctx.fillStyle = aliveColor;
    for (let y = 0; y < GRID_HEIGHT; y++) {
      for (let x = 0; x < GRID_WIDTH; x++) {
        if (grid[y][x] === 1) {
          ctx.fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
      }
    }

    if (showGridLines && CELL_SIZE > 2) {
      ctx.strokeStyle = GRID_LINE_COLOR;
      ctx.lineWidth = 0.5;
      for (let x = 0; x <= GRID_WIDTH; x++) {
        ctx.beginPath();
        ctx.moveTo(x * CELL_SIZE, 0);
        ctx.lineTo(x * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
        ctx.stroke();
      }
      for (let y = 0; y <= GRID_HEIGHT; y++) {
        ctx.beginPath();
        ctx.moveTo(0, y * CELL_SIZE);
        ctx.lineTo(GRID_WIDTH * CELL_SIZE, y * CELL_SIZE);
        ctx.stroke();
      }
    }
  }, [grid, aliveColor, deadColor, showGridLines]);

  return (
    <canvas
      ref={canvasRef}
      width={GRID_WIDTH * CELL_SIZE}
      height={GRID_HEIGHT * CELL_SIZE}
      className="max-w-full max-h-full object-contain rounded-md shadow-2xl shadow-black/50"
    />
  );
};

export default Grid;
