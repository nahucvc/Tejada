# /app/gui_dashboard.py
import os, math, threading, time

# Intento usar Tk; si falla (sin DISPLAY/paquetes), caigo a headless
try:
    import tkinter as tk
    from tkinter import ttk, TclError
    _HAS_TK = True
except Exception:
    _HAS_TK = False


class _Headless:
    def __init__(self):
        self._lock = threading.Lock()
        self._angle = 0.0
        self._ac = 0.0
        self._running = False

    def start(self):
        self._running = True
        print("[GUI] Headless: sin X11/Tk; registrando valores por consola.")

    def stop(self):
        self._running = False

    def set_angle(self, v: float):
        with self._lock:
            self._angle = float(v)
        print(f"[GUI] angle={self._angle:.1f}")

    def set_ac(self, v: float):
        with self._lock:
            self._ac = float(v)
        print(f"[GUI] ac={self._ac:.1f}")


class _TkDash:
    def __init__(self, title="Telemetría", w=420, h=320):
        self._lock = threading.Lock()
        self._angle = 0.0
        self._ac = 0.0
        self._running = False

        self._root = tk.Tk()
        self._root.title(title)
        self._root.geometry(f"{w}x{h}")
        self._root.protocol("WM_DELETE_WINDOW", self._on_close)

        tk.Label(self._root, text="Ángulo & Aceleración", font=("Arial", 12, "bold")).pack(pady=6)

        self._canvas_size = 200
        self._canvas = tk.Canvas(self._root, width=self._canvas_size, height=self._canvas_size)
        self._canvas.pack(pady=6)

        pad = 10
        self._cx = self._canvas_size // 2
        self._cy = self._canvas_size // 2
        self._r = (self._canvas_size // 2) - pad
        self._canvas.create_oval(self._cx - self._r, self._cy - self._r,
                                 self._cx + self._r, self._cy + self._r, outline="#444", width=2)
        for d in range(0, 360, 30):
            rad = math.radians(d)
            x1 = self._cx + (self._r - 10) * math.cos(rad)
            y1 = self._cy + (self._r - 10) * math.sin(rad)
            x2 = self._cx + self._r * math.cos(rad)
            y2 = self._cy + self._r * math.sin(rad)
            self._canvas.create_line(x1, y1, x2, y2, fill="#666")

        self._needle = self._canvas.create_line(self._cx, self._cy, self._cx, self._cy - (self._r - 15), width=3)
        self._angle_var = tk.StringVar(value="Ángulo: 0.0°")
        tk.Label(self._root, textvariable=self._angle_var).pack()

        frm = tk.Frame(self._root); frm.pack(fill="x", padx=12, pady=6)
        tk.Label(frm, text="Aceleración (0–100)").pack(anchor="w")
        self._ac_var = tk.DoubleVar(value=0.0)
        self._ac_bar = ttk.Progressbar(frm, variable=self._ac_var, orient="horizontal", length=360,
                                       mode="determinate", maximum=100.0)
        self._ac_bar.pack(fill="x", pady=4)
        self._ac_lbl = tk.StringVar(value="0.0")
        tk.Label(frm, textvariable=self._ac_lbl).pack(anchor="e")

    def _on_close(self):
        self._running = False
        try: self._root.destroy()
        except Exception: pass

    def _tick(self):
        with self._lock:
            ang = float(self._angle) % 360.0
            ac = max(0.0, min(100.0, float(self._ac)))

        rad = math.radians(ang)
        x = self._cx + (self._r - 15) * math.cos(rad)
        y = self._cy + (self._r - 15) * math.sin(rad)
        self._canvas.coords(self._needle, self._cx, self._cy, x, y)
        self._angle_var.set(f"Ángulo: {ang:.1f}°")
        self._ac_var.set(ac); self._ac_lbl.set(f"{ac:.1f}")

        if self._running:
            self._root.after(33, self._tick)

    def start(self):
        if self._running: return
        self._running = True
        self._root.after(33, self._tick)
        self._root.mainloop()

    def stop(self): self._on_close()
    def set_angle(self, v: float):
        with self._lock: self._angle = float(v)
    def set_ac(self, v: float):
        with self._lock: self._ac = float(v)


# ---------- API PÚBLICA ----------
_gui = None
_thread = None
_init_lock = threading.Lock()

def start_dashboard():
    """Inicia la GUI (o headless) en un hilo daemon. Idempotente."""
    global _gui, _thread
    with _init_lock:
        if _gui is not None:
            return _gui

        force_headless = os.getenv("GUI_HEADLESS", "").lower() in ("1","true","yes")
        use_headless = force_headless or (not _HAS_TK)

        if not use_headless and os.name != "nt":
            use_headless = ("DISPLAY" not in os.environ or not os.environ["DISPLAY"])

        if use_headless:
            _gui = _Headless(); _gui.start(); return _gui

        try:
            _gui = _TkDash()
        except Exception as e:  # p.ej. TclError si no hay DISPLAY
            print("[GUI] Tk no disponible; headless. Motivo:", e)
            _gui = _Headless(); _gui.start(); return _gui

        def _run():
            try:
                _gui.start()
            except Exception as e:
                print("[GUI] Error loop Tk; fallback headless:", e)
                fb = _Headless(); globals()["_gui"] = fb; fb.start()

        _thread = threading.Thread(target=_run, daemon=True)
        _thread.start()
        time.sleep(0.1)
        return _gui

def set_angle(angle: float):
    if _gui is None: start_dashboard()
    _gui.set_angle(angle)

def set_ac(ac: float):
    if _gui is None: start_dashboard()
    _gui.set_ac(ac)
