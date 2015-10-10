#! /usr/bin/env python
""" Deploys a .pth file in site-packages for easy importing """
import distutils.sysconfig
import os


def deploy():
    """Deploy gauss"""
    site = distutils.sysconfig.get_python_lib()
    pth = os.path.join(site, 'gauss.pth')

    if os.path.exists(pth):
        print("[i] Module already exists!")       # pylint: disable=C0325
    else:
        dirname = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        print dirname
        try:
            with open(pth, 'w') as stream:
                stream.write(dirname)
        except IOError:
            # pylint: disable=C0325
            print("[x] Please run this script with superuser privileges.")
            return

        print("[i] Testing module...")          # pylint: disable=C0325
        try:
            import gauss                        # pylint: disable=W0612
        except ImportError as exc:
            # pylint: disable=C0325
            print("Failed to deploy module! {}".format(exc))
        else:
            # pylint: disable=C0325
            print("[i] Module was successfully installed!")


if __name__ == "__main__":
    deploy()
