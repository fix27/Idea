import React, { useState, useCallback, useRef, useEffect } from 'react';
import Grid from './components/Grid';
import Controls from './components/Controls';
import {
  GRID_WIDTH,
  GRID_HEIGHT,
  INITIAL_PATTERN_STRING,
  NEIGHBOR_OFFSETS,
  BIRTH_RULES,
  SURVIVAL_RULES,
  ALIVE_COLOR,
  DEAD_COLOR,
} from './constants';

type GridType = number[][];

interface VisualizationOptions {
  aliveColor: string;
  deadColor: string;
  backgroundColor: string;
  showGridLines: boolean;
}

interface SimulationState {
  grid: GridType;
  generation: number;
}

const createEmptyGrid = (): GridType => {
  return Array.from({ length: GRID_HEIGHT }, () => Array(GRID_WIDTH).fill(0));
};

const createInitialGrid = (): GridType => {
  const grid = createEmptyGrid();
  const patternHeight = INITIAL_PATTERN_STRING.length;
  const patternWidth = INITIAL_PATTERN_STRING[0].length;

  const startY = Math.floor((GRID_HEIGHT - patternHeight) / 2);
  const startX = Math.floor((GRID_WIDTH - patternWidth) / 2);

  for (let y = 0; y < patternHeight; y++) {
    for (let x = 0; x < patternWidth; x++) {
      if (INITIAL_PATTERN_STRING[y][x] === '1') {
        grid[startY + y][startX + x] = 1;
      }
    }
  }
  return grid;
};

const countNeighbors = (grid: GridType, y: number, x: number): number => {
  let count = 0;
  for (const [dy, dx] of NEIGHBOR_OFFSETS) {
    const newY = y + dy;
    const newX = x + dx;

    // Check if the neighbor is within the grid boundaries.
    // This creates a non-wrapping grid, treating off-grid cells as dead.
    if (newY >= 0 && newY < GRID_HEIGHT && newX >= 0 && newX < GRID_WIDTH) {
      count += grid[newY][newX];
    }
  }
  return count;
};

const computeNextGeneration = (grid: GridType): GridType => {
  const nextGrid = createEmptyGrid();
  for (let y = 0; y < GRID_HEIGHT; y++) {
    for (let x = 0; x < GRID_WIDTH; x++) {
      const neighbors = countNeighbors(grid, y, x);
      const isAlive = grid[y][x] === 1;

      if (!isAlive && BIRTH_RULES.has(neighbors)) {
        nextGrid[y][x] = 1;
      } else if (isAlive && SURVIVAL_RULES.has(neighbors)) {
        nextGrid[y][x] = 1;
      }
    }
  }
  return nextGrid;
};

const App: React.FC = () => {
  // Fix: Use the recommended lazy initialization pattern for useState to ensure the grid is created only once.
  const [grid, setGrid] = useState<GridType>(() => createInitialGrid());
  const [isRunning, setIsRunning] = useState<boolean>(false);
  const [generation, setGeneration] = useState<number>(0);
  const [vizOptions, setVizOptions] = useState<VisualizationOptions>({
    aliveColor: ALIVE_COLOR,
    deadColor: DEAD_COLOR,
    backgroundColor: '#030712', // bg-gray-950
    showGridLines: true,
  });

  const gridRef = useRef<GridType>(grid);
  gridRef.current = grid;

  const animationFrameId = useRef<number>();

  const runSimulation = useCallback(() => {
    setGrid(g => computeNextGeneration(g));
    setGeneration(g => g + 1);

    animationFrameId.current = requestAnimationFrame(runSimulation);
  }, []);

  useEffect(() => {
    if (isRunning) {
      animationFrameId.current = requestAnimationFrame(runSimulation);
    } else {
      if (animationFrameId.current) {
        cancelAnimationFrame(animationFrameId.current);
      }
    }
    return () => {
      if (animationFrameId.current) {
        cancelAnimationFrame(animationFrameId.current);
      }
    };
  }, [isRunning, runSimulation]);

  const handlePlayPause = () => {
    setIsRunning(!isRunning);
  };

  const handleStep = () => {
    if (isRunning) setIsRunning(false);
    setGrid(g => computeNextGeneration(g));
    setGeneration(g => g + 1);
  };

  const handleReset = () => {
    setIsRunning(false);
    setGrid(createInitialGrid());
    setGeneration(0);
  };

  const handleVizChange = (newOptions: Partial<VisualizationOptions>) => {
    setVizOptions(prev => ({ ...prev, ...newOptions }));
  };

  const handleSave = () => {
    const state: SimulationState = {
      grid: gridRef.current,
      generation,
    };
    const blob = new Blob([JSON.stringify(state, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `life_b356_s2_state_gen_${generation}.json`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  const handleLoad = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (e) => {
      try {
        const text = e.target?.result;
        if (typeof text !== 'string') throw new Error("File is not text");
        const state = JSON.parse(text) as SimulationState;
        
        if (state.grid && state.generation !== undefined && Array.isArray(state.grid)) {
          setIsRunning(false);
          setGrid(state.grid);
          setGeneration(state.generation);
        } else {
          alert('Invalid save file format.');
        }
      } catch (error) {
        console.error("Failed to load state:", error);
        alert('Failed to read or parse the save file.');
      }
    };
    reader.readAsText(file);
    event.target.value = '';
  };

  return (
    <div className="flex flex-col h-screen font-sans transition-colors duration-300" style={{ backgroundColor: vizOptions.backgroundColor }}>
      <header className="p-4 text-center border-b border-gray-800 shadow-lg">
        <h1 className="text-2xl md:text-3xl font-bold text-emerald-400 tracking-wider">Life B356/S2</h1>
        <p className="text-sm text-gray-400 mt-1">A high-performance Game of Life simulation.</p>
      </header>
      
      <main className="flex-grow flex items-center justify-center p-4 overflow-hidden">
        <Grid 
          grid={grid} 
          aliveColor={vizOptions.aliveColor}
          deadColor={vizOptions.deadColor}
          showGridLines={vizOptions.showGridLines}
        />
      </main>

      <footer className="w-full p-4 bg-gray-900/50 backdrop-blur-sm border-t border-gray-800">
        <Controls
          isRunning={isRunning}
          generation={generation}
          onPlayPause={handlePlayPause}
          onStep={handleStep}
          onReset={handleReset}
          vizOptions={vizOptions}
          onVizChange={handleVizChange}
          onSave={handleSave}
          onLoad={handleLoad}
        />
      </footer>
    </div>
  );
};

export default App;