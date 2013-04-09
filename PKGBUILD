pkgname=runescape-client-git
pkgbase=runescape-client
pkgver=2013.03.28
pkgrel=1
pkgdesc="Runescape Client for Linux"
arch=(any)
url="https://github.com/Unia/runescape-client"
license=(GPL2)
depends=('java-runtime' 'curl' 'p7zip' 'gtk3' 'webkitgtk3')
optdepends=('pulseaudio: have sound played through pulseaudio'
			'alsa-oss: have sound played through alsa')
conflicts=('runescape-client-bin' 'unix-runescape-client')

_gitroot="https://github.com/Unia/runescape-client"
_gitname="$pkgbase"

pkgver() {
    cd "$srcdir/$_gitname"
    git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
  cd "$srcdir"
  msg "Connecting to GIT server..."

  if [ -d ${_gitname} ] ; then
    cd ${_gitname}
    git pull
    msg "The local files are updated."
  else
    git clone ${_gitroot} ${_gitname}
  fi
  msg "GIT checkout done or server timeout"
}

package() {
  cd "$srcdir/$pkgbase"
  make all
  make DESTDIR="$pkgdir" install
}
md5sums=()
