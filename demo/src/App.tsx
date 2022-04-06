import React, { useCallback, useEffect, useRef, useState } from 'react';
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

const size = 1

function App() {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const [width, setWidth] = useState(512)
  const [height, setHeight] = useState(512)
  const [rafId, setRafId] = useState<number | null>(null)
  const [duration, setDuration] = useState(0)

  const wasmState = useAsync(async () => {
    const wasm = createWasmModule()
    return wasm
  })

  useEffect(() => {
    if (wasmState.value && canvasRef && canvasRef.current) {
      if (rafId) {
        cancelAnimationFrame(rafId)
      }

      const wasm = wasmState.value
      wasm._initialize(width, height, size, Date.now() % 10000)
      console.log("Initialize done")

      const outputImageBufferOffset = wasm._getVisualizeBufferOffset() 
      const outImageData = new ImageData(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * size * height * size * 4)), width * size, height * size)
      const canvas = canvasRef.current
      canvas.getContext('2d')!.putImageData(outImageData, 0, 0)

      const inputBufferOffset = wasm._getInputBufferOffset()
      console.log(new Uint8ClampedArray(wasm.HEAPU8.slice(inputBufferOffset, inputBufferOffset + width * height)))
      console.log(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * height * 4)))

      const render = () => {
        if (!wasmState.value) return
        const wasm = wasmState.value
        const start = performance.now()
        const dur = wasmState.value!._exec(width, height, size)
        const outImageData = new ImageData(new Uint8ClampedArray(wasm.HEAPU8.slice(outputImageBufferOffset, outputImageBufferOffset + width * size * height * size * 4)), width * size, height * size)
        canvasRef.current?.getContext('2d')?.putImageData(outImageData, 0, 0)
        // console.log(dur)
        const stop = performance.now()
        const dur2 = stop - start
        setDuration(dur2)
        setRafId(requestAnimationFrame(render))
      }
      setRafId(requestAnimationFrame(render))
    }
  }, [width, height, wasmState, canvasRef])

  return (
    <div className="App">
      <div>
        <select onChange={ev => {
          const val = ev.target.value
          const xPos = val.indexOf("x")
          const w = parseInt(val.substring(0, xPos))
          const h = parseInt(val.substring(xPos + 1))
          console.log(w, h)
          setWidth(w)
          setHeight(h)
        }}>
          <option selected>512x512</option>
          <option>1024x1024</option>
          <option>2048x2048</option>
          <option>4096x4096</option>
        </select>
        <span>{duration.toFixed(3)} ms</span>
      </div>
      <canvas ref={canvasRef} width={width * size} height={height * size}></canvas>
    </div>
  );
}

export default App;
