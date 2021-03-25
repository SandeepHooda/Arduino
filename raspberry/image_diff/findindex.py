def index(src, search):
    try:
        if (src.index(search) >= 0 ):
            return src.index(search);
    except ValueError as ve:
        return -1;