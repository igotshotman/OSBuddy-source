# Maintainer: Unia <jthidskes@outlook.com> 

pkgname=runescape-client-git
_gitname=runescape-client
pkgver=2013.04.10
pkgrel=1
pkgdesc="Runescape Client for Linux"
arch=('i686' 'x86_64')
url="https://github.com/Unia/runescape-client"
license=('GPL2')
depends=('java-runtime' 'curl' 'p7zip' 'gtk3' 'webkitgtk3')
makedepends=('git')
optdepends=('pulseaudio: have sound played through pulseaudio'
            'alsa-oss: have sound player through alsa')
source=('git://github.com/Unia/runescape-client.git')
md5sums=('SKIP')

pkgver() {
  cd $_gitname
  git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
  cd $_gitname
  make
}

package() {
  cd $_gitname
  make PREFIX=/usr DESTDIR="$pkgdir" install
}
