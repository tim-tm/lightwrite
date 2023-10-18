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
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

### NOTE: Do not rely on the codebase, this is a beginner project made for education, things may and will heavily change.

This is a really simple GUI-based text editor, which is based on the dramatic editor [ded](https://github.com/tsoding/ded), developed by [tsoding](https://github.com/tsoding), a twitch streamer known for recreational programming. As of right now, lightwrite is using his approach of handling file-content. Even though the code for managing files is not exactly the same, lightwrite will at least use the same datastructures.

* Written in plain C.
* lightweight (as the name implies)
* simple codebase (only GNU-Make as a build tool since CMake would be unnecessary)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![SDL2][SDL]][SDL-url]
* [![SDL2_ttf][SDL_ttf]][SDL_ttf-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

Here you can see how to setup this project locally.

### Prerequisites

Since this project is mainly developed in Linux, Windows build is not yet supported.
Currently this project requires at least GNU-Make version 4.4 since the Makefile is using the .WAIT feature. I would suggest building Make from source, alternatively use a newer pre-built version , [pacman](https://archlinux.org/pacman/) for example provides a newer version of make [pacman-make](https://archlinux.org/packages/core/x86_64/make/).

* [install and setup git](https://git-scm.com/book/en/v2)
* [gnu make](https://www.gnu.org/software/make/#download)
* [gnu compiler collection (gcc/g++)](https://gcc.gnu.org/)
* [SDL2](https://wiki.libsdl.org/SDL2/Installation)
* [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases)

### Installation

You could also globally install each dependency, to be ready for compilation.
Arch Linux example:
```sh
sudo pacman -Syu sdl2 sdl2_ttf
```

General setup.
* clone the repo and move inside
  ```sh
  git clone git@github.com:tim-tm/lightwrite.git && cd breakout
  ```
* setting up the project
  ```sh
  make setup
  ```
* building
  ```sh
  make
  ```

If you want to clean the build directory run one of the following commands:
* only cleaning
  ```sh
  make clean
  ```
* cleaning, checking the setup, rebuilding
  ```sh
  make rebuild
  ```

There is also a command for destroying the setup (aka. deleting the entire build directory)
```sh
make destroy
```

Additionally, if you want to compile a release build without any debug features, specify the following options to make:
* performing a normal build
  ```sh
  make BUILD_TYPE=RELEASE
  ```
* performing a entire rebuild
  ```sh
  make rebuild BUILD_TYPE=RELEASE
  ```

Here you can see the folder structure of a built project.
```sh
    .
    ├── build
    │   ├── release             # Results of a release compilation will be stored here.
    │   │   └── ...             # Exactly the same structure as in build.
    │   ├── resource            # Copy of the resources (done by the Makefile).
    │   ├── lightwrite          # The debug executable.
    │   ├── main.o              # Compiled source files.
    │   └── ...                 # More compiled source files.
    ├── src                     # All source files.
    ├── resource
    │   └── AnyFont.ttf         # By default MonoLisaRegular is used but you could change that to any ttf font. (font selection inside of the editor coming soon)
    ├── Makefile
    ├── LICENSE
    └── README.md
```

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
