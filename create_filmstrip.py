#!/usr/bin/env python3
"""
Genera un knob filmstrip PNG de ejemplo para probar en Custom DAW
El filmstrip tiene 64 frames (0-360 grados dividido en pasos de ~5.6 grados)
"""

from PIL import Image, ImageDraw
import math

# Configuración
FRAME_SIZE = 100  # Tamaño de cada frame (100x100px)
NUM_FRAMES = 64   # Número de frames
OUTPUT_FILE = "example_knob_filmstrip.png"

# Crear imagen vertical (ancho=100, alto=100*64=6400)
filmstrip = Image.new('RGBA', (FRAME_SIZE, FRAME_SIZE * NUM_FRAMES), (0, 0, 0, 0))

for frame_idx in range(NUM_FRAMES):
    # Calcular ángulo para este frame (-135° a +135° = 270° total)
    # Esto corresponde al rango típico de un knob rotatorio
    start_angle = -135  # degrees
    end_angle = 135     # degrees
    total_range = end_angle - start_angle
    
    angle = start_angle + (frame_idx / (NUM_FRAMES - 1)) * total_range
    angle_rad = math.radians(angle)
    
    # Crear frame individual
    frame = Image.new('RGBA', (FRAME_SIZE, FRAME_SIZE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(frame)
    
    center_x = FRAME_SIZE // 2
    center_y = FRAME_SIZE // 2
    radius = 40
    
    # Dibujar círculo exterior (cuerpo del knob)
    outer_color = (50, 180, 255, 255)  # Azul cyan
    draw.ellipse([center_x - radius, center_y - radius, 
                  center_x + radius, center_y + radius], 
                 fill=outer_color, outline=(30, 130, 200, 255), width=3)
    
    # Dibujar arco de progreso (0 a posición actual)
    progress_angle = angle
    if progress_angle < start_angle:
        progress_angle = start_angle
    
    # Calcular puntos del arco
    arc_thickness = 6
    inner_radius = radius - arc_thickness - 2
    
    # Dibujar indicador (línea desde centro hacia afuera)
    indicator_length = radius - 5
    end_x = center_x + indicator_length * math.cos(angle_rad - math.pi/2)
    end_y = center_y + indicator_length * math.sin(angle_rad - math.pi/2)
    
    draw.line([center_x, center_y, end_x, end_y], 
              fill=(255, 255, 255, 255), width=4)
    
    # Dibujar punto en el extremo del indicador
    dot_radius = 3
    draw.ellipse([end_x - dot_radius, end_y - dot_radius,
                  end_x + dot_radius, end_y + dot_radius],
                 fill=(255, 255, 255, 255))
    
    # Dibujar círculo central pequeño
    center_radius = 8
    draw.ellipse([center_x - center_radius, center_y - center_radius,
                  center_x + center_radius, center_y + center_radius],
                 fill=(30, 130, 200, 255))
    
    # Pegar este frame en el filmstrip
    y_offset = frame_idx * FRAME_SIZE
    filmstrip.paste(frame, (0, y_offset))

# Guardar filmstrip
filmstrip.save(OUTPUT_FILE)
print(f"Filmstrip creado: {OUTPUT_FILE}")
print(f"Dimensiones: {FRAME_SIZE}x{FRAME_SIZE * NUM_FRAMES} ({NUM_FRAMES} frames)")
print(f"\nPara usar:")
print(f"1. Abre Custom DAW")
print(f"2. Arrastra un Knob al canvas")
print(f"3. Selecciónalo")
print(f"4. En Property Inspector, cambia 'Knob Style' a 'Filmstrip'")
print(f"5. Haz clic en 'Cargar PNG...' y selecciona '{OUTPUT_FILE}'")
