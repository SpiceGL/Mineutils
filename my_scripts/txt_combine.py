from mineutils.path import Path as pt


def combine(txt_paths, add_chapter_name=False, start_chapter=1):
    save_name = "combined.txt"
    chaptet_id = start_chapter
    with open(save_name, "w", encoding="utf8") as save_file:
        for txt_pt in txt_paths:
            with open(txt_pt, "r") as f:
                lines = f.readlines()
                if add_chapter_name:
                    chaptet_name = f"第{str(chaptet_id).zfill(4)}章\n"
                    chaptet_id += 1
                else:
                    chaptet_name = "\n"
                save_file.write(chaptet_name)
                save_file.writelines(lines)
                save_file.write("\n\n")
                    

def main():
    txt_dirpath = ""
    txt_paths = pt.listDir(txt_dirpath)
    combine(txt_paths)


if __name__ == '__main__':
    main()