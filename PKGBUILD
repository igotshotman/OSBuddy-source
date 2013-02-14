pkgname=runescape-client-git
pkgbase=runescape-client
pkgver=20130212
pkgrel=1
pkgdesc="Runescape Client for Linux"
arch=(any)
url="https://github.com/Unia/runescape-client"
license=(GPL2)
depends=('java-runtime' 'curl' 'perl' 'p7zip' 'gtk3' 'webkitgtk3')
optdepends=()
conflicts=('runescape-client-bin' 'unix-runescape-client')

_gitroot="https://github.com/Unia/runescape-client"
_gitname="$pkgbase"

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
