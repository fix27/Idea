
import React, { useState } from 'react';

interface VisualizationOptions {
  aliveColor: string;
  deadColor: string;
  backgroundColor: string;
  showGridLines: boolean;
}

interface ControlsProps {
  isRunning: boolean;
  generation: number;
  onPlayPause: () => void;
  onStep: () => void;
  onReset: () => void;
  vizOptions: VisualizationOptions;
  onVizChange: (newOptions: Partial<VisualizationOptions>) => void;
  onSave: () => void;
  onLoad: (event: React.ChangeEvent<HTMLInputElement>) => void;
}

const PlayIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
    <path fillRule="evenodd" d="M4.5 5.653c0-1.426 1.529-2.33 2.779-1.643l11.54 6.647c1.295.742 1.295 2.545 0 3.286L7.279 20.99c-1.25.717-2.779-.217-2.779-1.643V5.653z" clipRule="evenodd" />
  </svg>
);

const PauseIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
    <path fillRule="evenodd" d="M6.75 5.25a.75.75 0 00-.75.75v12c0 .414.336.75.75.75h2.25a.75.75 0 00.75-.75v-12a.75.75 0 00-.75-.75H6.75zm6 0a.75.75 0 00-.75.75v12c0 .414.336.75.75.75h2.25a.75.75 0 00.75-.75v-12a.75.75 0 00-.75-.75H12.75z" clipRule="evenodd" />
  </svg>
);

const StepIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
        <path fillRule="evenodd" d="M16.5 5.25a.75.75 0 00-.75.75v12c0 .414.336.75.75.75h2.25a.75.75 0 00.75-.75v-12a.75.75 0 00-.75-.75H16.5z" clipRule="evenodd" />
        <path fillRule="evenodd" d="M4.5 5.653c0-1.426 1.529-2.33 2.779-1.643l6.054 3.475a2.75 2.75 0 010 4.986l-6.054 3.475c-1.25.717-2.779-.217-2.779-1.643V5.653z" clipRule="evenodd" />
    </svg>
);

const ResetIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
        <path fillRule="evenodd" d="M4.755 10.059a7.5 7.5 0 0112.548-3.364l1.903 1.903h-4.5a.75.75 0 00-.75.75v4.5l1.903-1.903a5.25 5.25 0 10-9.279 3.646.75.75 0 10-1.49-.175a6.75 6.75 0 0110.77-4.425a.75.75 0 00-.584-1.245A6.75 6.75 0 014.755 10.059z" clipRule="evenodd" />
    </svg>
);

const SettingsIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
    <path fillRule="evenodd" d="M11.078 2.25c-.917 0-1.699.663-1.85 1.567L9.05 5.85c-.09.55-.525.954-1.095 1.034-1.136.153-2.132.82-2.735 1.772a.75.75 0 00.278 1.134l1.358.815c.488.293.713.886.564 1.425a3.989 3.989 0 000 2.83c.149.539-.076 1.132-.564 1.425l-1.358.815a.75.75 0 00-.278 1.134c.603.952 1.599 1.62 2.735 1.772.57.08.995.484 1.095 1.034l.178 2.033c.151.904.933 1.567 1.85 1.567h1.844c.917 0 1.699-.663 1.85-1.567l.178-2.034c.09-.55.525-.954 1.095-1.034 1.136-.153 2.132-.82 2.735-1.772a.75.75 0 00-.278-1.134l-1.358-.815c-.488-.293-.713-.886-.564-1.425a3.989 3.989 0 000-2.83c-.149-.539.076-1.132.564-1.425l1.358-.815a.75.75 0 00.278-1.134c-.603-.952-1.599-1.62-2.735-1.772-.57-.08-.995-.484-1.095-1.034L13.173 3.817c-.151-.904-.933-1.567-1.85-1.567h-1.844zM12 15.75a3.75 3.75 0 100-7.5 3.75 3.75 0 000 7.5z" clipRule="evenodd" />
  </svg>
);

const SaveIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
        <path d="M3 3.75A.75.75 0 013.75 3h16.5a.75.75 0 01.75.75v16.5a.75.75 0 01-.75.75H3.75a.75.75 0 01-.75-.75V3.75z" opacity=".25" />
        <path d="M12 18a.75.75 0 00.75-.75V7.56l1.22 1.22a.75.75 0 101.06-1.06l-2.5-2.5a.75.75 0 00-1.06 0l-2.5 2.5a.75.75 0 101.06 1.06l1.22-1.22v9.69A.75.75 0 0012 18z" />
    </svg>
);

const LoadIcon: React.FC<React.SVGProps<SVGSVGElement>> = (props) => (
    <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" {...props}>
        <path d="M3 3.75A.75.75 0 013.75 3h16.5a.75.75 0 01.75.75v16.5a.75.75 0 01-.75.75H3.75a.75.75 0 01-.75-.75V3.75z" opacity=".25" />
        <path d="M12 6a.75.75 0 00-.75.75v9.69l-1.22-1.22a.75.75 0 00-1.06 1.06l2.5 2.5a.75.75 0 001.06 0l2.5-2.5a.75.75 0 10-1.06-1.06l-1.22 1.22V6.75A.75.75 0 0012 6z" />
    </svg>
);

const Controls: React.FC<ControlsProps> = ({ isRunning, generation, onPlayPause, onStep, onReset, vizOptions, onVizChange, onSave, onLoad }) => {
  const [showOptions, setShowOptions] = useState(false);
  
  return (
    <div className="relative w-full max-w-5xl mx-auto">
        <div className="flex flex-col md:flex-row items-center justify-center gap-4 md:gap-6 w-full">
            <div className="flex items-center gap-2">
                <button onClick={onPlayPause} className="p-3 rounded-full bg-emerald-500 text-white hover:bg-emerald-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-emerald-400 transition-colors">
                    {isRunning ? <PauseIcon className="w-6 h-6" /> : <PlayIcon className="w-6 h-6" />}
                </button>
                <button onClick={onStep} disabled={isRunning} className="p-3 rounded-full bg-gray-700 text-gray-200 hover:bg-gray-600 disabled:opacity-50 disabled:cursor-not-allowed focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-indigo-400 transition-colors">
                    <StepIcon className="w-6 h-6" />
                </button>
                <button onClick={onReset} className="p-3 rounded-full bg-gray-700 text-gray-200 hover:bg-gray-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-rose-400 transition-colors">
                    <ResetIcon className="w-6 h-6" />
                </button>
            </div>
            <div className="text-center md:text-left bg-gray-800 px-4 py-2 rounded-lg">
                <span className="text-sm font-medium text-gray-400">Generation</span>
                <p className="text-2xl font-mono font-bold text-gray-50 tracking-wider">
                    {generation.toLocaleString()}
                </p>
            </div>
             <div className="flex items-center gap-2">
                 <button onClick={onSave} title="Save State" className="p-3 rounded-full bg-gray-700 text-gray-200 hover:bg-gray-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-sky-400 transition-colors">
                    <SaveIcon className="w-6 h-6" />
                </button>
                <label htmlFor="load-state-input" title="Load State" className="p-3 rounded-full bg-gray-700 text-gray-200 hover:bg-gray-600 cursor-pointer focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-sky-400 transition-colors">
                    <LoadIcon className="w-6 h-6" />
                </label>
                <input id="load-state-input" type="file" accept=".json" className="hidden" onChange={onLoad} />
                <button onClick={() => setShowOptions(!showOptions)} title="Visualization Settings" className="p-3 rounded-full bg-gray-700 text-gray-200 hover:bg-gray-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-offset-gray-900 focus:ring-purple-400 transition-colors">
                    <SettingsIcon className="w-6 h-6" />
                </button>
            </div>
        </div>

        {showOptions && (
            <div className="absolute bottom-full mb-4 w-full left-0 right-0 p-4 bg-gray-800 rounded-lg shadow-lg border border-gray-700 animate-fade-in-up">
                <h3 className="text-lg font-semibold text-gray-200 mb-3 text-center">Visualization Options</h3>
                <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-4 gap-4 text-sm">
                    <div className="flex flex-col gap-2">
                        <label htmlFor="aliveColor" className="font-medium text-gray-400">Alive Cell</label>
                        <input type="color" id="aliveColor" value={vizOptions.aliveColor} onChange={e => onVizChange({ aliveColor: e.target.value })} className="w-full h-10 p-1 bg-gray-700 border border-gray-600 rounded-md cursor-pointer"/>
                    </div>
                     <div className="flex flex-col gap-2">
                        <label htmlFor="deadColor" className="font-medium text-gray-400">Dead Cell</label>
                        <input type="color" id="deadColor" value={vizOptions.deadColor} onChange={e => onVizChange({ deadColor: e.target.value })} className="w-full h-10 p-1 bg-gray-700 border border-gray-600 rounded-md cursor-pointer"/>
                    </div>
                     <div className="flex flex-col gap-2">
                        <label htmlFor="backgroundColor" className="font-medium text-gray-400">Background</label>
                        <input type="color" id="backgroundColor" value={vizOptions.backgroundColor} onChange={e => onVizChange({ backgroundColor: e.target.value })} className="w-full h-10 p-1 bg-gray-700 border border-gray-600 rounded-md cursor-pointer"/>
                    </div>
                    <div className="flex items-center justify-center mt-4 sm:mt-0 sm:items-end sm:pb-1">
                         <label htmlFor="showGridLines" className="font-medium text-gray-400 select-none cursor-pointer flex items-center gap-2">
                            <input type="checkbox" id="showGridLines" checked={vizOptions.showGridLines} onChange={e => onVizChange({ showGridLines: e.target.checked })} className="w-5 h-5 rounded bg-gray-700 border-gray-600 text-emerald-500 focus:ring-emerald-500"/>
                            Show Grid
                        </label>
                    </div>
                </div>
            </div>
        )}
    </div>
  );
};

export default Controls;
