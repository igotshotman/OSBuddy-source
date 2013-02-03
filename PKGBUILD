pkgname=runescape-client-git
pkgbase=runescape-client
pkgver=20130203
pkgrel=1
pkgdesc="Runescape Client for Linux"
arch=(any)
url="https://github.com/Unia/runescape-client"
license=(GPL2)
depends=('java-runtime' 'curl' 'perl' 'p7zip')
optdepends=()
install='runescape-client.install'
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
  make runescape-launcher
  make runescape-update-client
  make DESTDIR="$pkgdir" install
}
md5sums=()
