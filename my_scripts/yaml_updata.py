from mineutils.fileio import Yaml
from mineutils.colorstr import ColorStr as cs
from mineutils.path import Path as pt


def yamlKeyUpdata(yaml_path, old_key, new_key):
    content = Yaml.load(yaml_path)
    if old_key in content:
        content[new_key] = content.pop(old_key)
        print(cs.green(f"{yaml_path}:"), f"将{cs.fuchsia(old_key)}替换为{cs.fuchsia(new_key)}")
    Yaml.save(yaml_path, content)


def main():
    base_path = "Models"
    dir_paths = pt.listDir(base_path)
    for dir_path in dir_paths:
        yaml_path = pt.join(dir_path, "train_config.yaml")
        if pt.exists(yaml_path):
            yamlKeyUpdata(yaml_path, "model_init", "net_init")
            yamlKeyUpdata(yaml_path, "model_name", "net_name")


if __name__ == "__main__":
    from mineutils import useProjDir
    useProjDir(splitsrt="my_tools")
    main()
