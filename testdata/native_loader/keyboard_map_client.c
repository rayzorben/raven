/* MapVirtualKeyExW and MapVirtualKeyExA map virtual keys, scan codes and characters on the US layout, as OpenMPT's key map does
   with the thread's layout, answer the same with no layout, return 0 for a code or a mapping type they do not know, and agree with
   MapVirtualKeyW and MapVirtualKeyA. */
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#include <windows.h>

int raven_entry(void)
{
	const HKL layout = GetKeyboardLayout(0);
	if (MapVirtualKeyExW('A', MAPVK_VK_TO_VSC, layout) != 0x1e || MapVirtualKeyExW(VK_RETURN, MAPVK_VK_TO_VSC, layout) != 0x1c)
		return 1;
	if (MapVirtualKeyExW(VK_LSHIFT, MAPVK_VK_TO_VSC, layout) != 0x2a || MapVirtualKeyExW('A', MAPVK_VK_TO_VSC_EX, layout) != 0x1e)
		return 2;
	if (MapVirtualKeyExW(0x1e, MAPVK_VSC_TO_VK, layout) != 'A' || MapVirtualKeyExW(0x39, MAPVK_VSC_TO_VK_EX, layout) != VK_SPACE)
		return 3;
	if (MapVirtualKeyExW('A', MAPVK_VK_TO_CHAR, layout) != 'A' || MapVirtualKeyExW(VK_OEM_2, MAPVK_VK_TO_CHAR, layout) != '/')
		return 4;
	if (MapVirtualKeyExW('A', MAPVK_VK_TO_VSC, NULL) != 0x1e || MapVirtualKeyExW(0x1e, MAPVK_VSC_TO_VK, NULL) != 'A')
		return 5;
	if (MapVirtualKeyExW(0xff, MAPVK_VK_TO_VSC, layout) || MapVirtualKeyExW(0x200, MAPVK_VSC_TO_VK, layout) || MapVirtualKeyExW('A', 9, layout))
		return 6;
	if (MapVirtualKeyExA('A', MAPVK_VK_TO_VSC, layout) != 0x1e || MapVirtualKeyExA(0x1e, MAPVK_VSC_TO_VK, layout) != 'A' ||
		MapVirtualKeyExA(VK_OEM_2, MAPVK_VK_TO_CHAR, NULL) != '/')
		return 7;
	if (MapVirtualKeyExA(0xff, MAPVK_VK_TO_VSC, layout) || MapVirtualKeyExA(0x200, MAPVK_VSC_TO_VK, layout))
		return 8;
	for (UINT type = MAPVK_VK_TO_VSC; type <= MAPVK_VSC_TO_VK_EX; ++type)
		for (UINT code = 0; code < 256; ++code)
			if (MapVirtualKeyExW(code, type, layout) != MapVirtualKeyW(code, type) ||
				MapVirtualKeyExA(code, type, layout) != MapVirtualKeyA(code, type))
				return 9;
	return 0;
}
