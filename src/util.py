def hex_to_norm_rgba(hex_color:str, round_to:int = 1)->tuple[float,float,float,float]:
    hex_color = hex_color.lstrip('#')
    r = int(hex_color[0:2], 16)
    g = int(hex_color[2:4], 16)
    b = int(hex_color[4:6], 16)
    a = int(hex_color[6:8], 16) if len(hex_color) > 6 else 255
    return (round(r / 255.0, round_to), round(g / 255.0, round_to), round(b / 255.0, round_to), round(a / 255.0, round_to))


def norm_rgba_to_hex(rgba:tuple[float,float,float,float])->str:
    r = int(rgba[0] * 255)
    g = int(rgba[1] * 255)
    b = int(rgba[2] * 255)
    a = int(rgba[3] * 255)
    return f'#{r:02X}{g:02X}{b:02X}{a:02X}'


def main():
    vertexColor = (0.5, 0.0, 0.0, 1.0)
    print(f"Vertex Color: {norm_rgba_to_hex(vertexColor)}")

if __name__ == "__main__":
    main()