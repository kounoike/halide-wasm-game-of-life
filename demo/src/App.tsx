import React, { useEffect, useRef, useState } from 'react';
import './App.css';
import {useAsync} from 'react-use'

export interface Wasm extends EmscriptenModule {
  _getInputBufferOffset(): number
  _getOutputBufferOffset(): number
  _getVisualizeBufferOffset(): number
  _initialize(width: number, height: number, size: number, seed: number): void
  _exec(width: number, height: number, size: number): number
}

declare function createWasmModule(): Promise<Wasm>

const width = 1024
const height = 1024
const size = 1

function App() {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const [duration, setDuration] = useState(0)
  const wasmState = useAsync(async () => {
    const wasm = createWasmModule()
    return wasm
  })

  useEffect(() => {
    if (!wasmState.loading && canvasRef.current) {
      const wasm = wasmState.value!
      wasm._initialize(width, height, size, Date.now())
      console.log("Initialize done")

      const outputImageBufferOffset = wasm._getVisualizeBufferOffset() 
      const outImageData = new ImageData(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * size * height * size * 4)), width * size, height * size)
      const canvas = canvasRef.current
      canvas.getContext('2d')!.putImageData(outImageData, 0, 0)

      const inputBufferOffset = wasm._getInputBufferOffset()
      console.log(new Uint8ClampedArray(wasm.HEAPU8.slice(inputBufferOffset, inputBufferOffset + width * height)))
      console.log(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * height * 4)))

      const render = () => {
        const start = performance.now()
        const dur = wasm._exec(width, height, size)
        const outImageData = new ImageData(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * size * height * size * 4)), width * size, height * size)
        canvasRef.current?.getContext('2d')?.putImageData(outImageData, 0, 0)
        // console.log(dur)
        const stop = performance.now()
        const dur2 = stop - start
        setDuration(dur2)
        requestAnimationFrame(render)
      }
      render()
    }
  }, [wasmState])

  return (
    <div className="App">
      <canvas ref={canvasRef} width={width * size} height={height * size}></canvas>
      <div>
        <p>{duration} ms</p>
      </div>
    </div>
  );
}

export default App;
