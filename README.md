<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">lightwrite</h3>

  <p align="center">
    Little text editor written in C using SDL2.
    <br />
    <a href="https://github.com/tim-tm/lightwrite/issues">Report Bug</a>
    ·
    <a href="https://github.com/tim-tm/lightwrite/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#using">Using</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

This is a really simple GUI-based text editor, which is based on the dramatic editor [ded](https://github.com/tsoding/ded), developed by [tsoding](https://github.com/tsoding), a twitch streamer known for recreational programming. As of right now, lightwrite is using his approach of handling file-content. Even though the code for managing files is not exactly the same, lightwrite will at least use the same datastructures.

* Written in plain C.
* lightweight (as the name implies)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

[![Packaging status](https://repology.org/badge/vertical-allrepos/lightwrite.svg)](https://repology.org/project/lightwrite/versions)

If you only want to use lightwrite and not mess with it's source, go ahead an grab one of the [avalible releases](https://github.com/tim-tm/lightwrite/releases/latest) (preferably the latest).

prepare the release folder (VERSION stands for the version you've downloaded)
```sh
tar -xf lightwrite-VERSION.tar.gz && cd lightwrite-VERSION
```

building
```sh
./configure && make
```

installing
```sh
sudo make install
```

### Messing with the source

Here you can see how to setup this project locally.
Since this project is mainly developed in Linux, Windows build is not yet supported.

### Prerequisites

* git, autotools, make, gcc
* sdl2, sdl2_ttf

### Installation

clone the repo and move inside
```sh
git clone git@github.com:tim-tm/lightwrite.git && cd breakout
```

setting up the project
```sh
./autogen.sh
```

building
```sh
make -C build
```
or
```sh
cd build && make
```

Cleanup
```sh
./cleanup.sh
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Using

Lightwrite is **not** a vim-like editor (even though I personally prefer such editors).

|     **Keybind**    | **Action**                                  |
|:------------------:|---------------------------------------------|
| Ctrl+S             | Save the current buffer                     |
| Ctrl+Shift+f       | Open the filemanager                        |
| Ctrl+a             | Create a new file (only in the filemanager) |
| Up/Down/Left/Right | Navigating up/down/left/right               |

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.
SDL2 and SDL2_ttf are both distributed under the [zlib license](https://www.zlib.net/zlib_license.html).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/tim-tm/lightwrite.svg?style=for-the-badge
[contributors-url]: https://github.com/tim-tm/lightwrite/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/tim-tm/lightwrite.svg?style=for-the-badge
[forks-url]: https://github.com/tim-tm/lightwrite/network/members
[stars-shield]: https://img.shields.io/github/stars/tim-tm/lightwrite.svg?style=for-the-badge
[stars-url]: https://github.com/tim-tm/lightwrite/stargazers
[issues-shield]: https://img.shields.io/github/issues/tim-tm/lightwrite.svg?style=for-the-badge
[issues-url]: https://github.com/tim-tm/lightwrite/issues
[license-shield]: https://img.shields.io/github/license/tim-tm/lightwrite.svg?style=for-the-badge
[license-url]: https://github.com/tim-tm/lightwrite/blob/main/LICENSE
[SDL]: https://img.shields.io/badge/SDL-35495E?style=for-the-badge&logo=&logoColor=4FC08D
[SDL-url]: https://www.libsdl.org/
[SDL_ttf]: https://img.shields.io/badge/SDL_ttf-35495E?style=for-the-badge&logo=&logoColor=4FC08D
[SDL_ttf-url]: https://wiki.libsdl.org/SDL2_ttf/FrontPage
