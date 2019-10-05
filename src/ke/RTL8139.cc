#include <RTL8139.h>
#include <kernel.h>
#include <system.h>
#include <pci.h>

uint16_t rx_buffer[8192];

void rtl_init() {
	// First get the network device using PCI
    pci_dev_t pci_rtl8139_device = pci_get_device(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, -1);
    
    if (!pci_compare(pci_rtl8139_device, dev_zero)) {
	    uint32_t ret = pci_read(pci_rtl8139_device, PCI_BAR0);
	    uint32_t bar_type = ret & 0x1;
	    // Get io base or mem base by extracting the high 28/30 bits
	    uint32_t io_base = ret & (~0x3);
	    uint32_t mem_base = ret & (~0xf);
	    term.printf("rtl8139 uses %s access (base: 0x%x)\n", (bar_type == 0)? "mem based":"port based", (bar_type != 0)?io_base:mem_base);

	    // IO reset
	    outportb(io_base + 0x52, 0x0);
	    outportb(io_base + 0x47, 0x10);

	    // init recieve buffer
	    outportl(io_base + 0x30, (uintptr_t)rx_buffer);

	    // init IRQ
	    outportw(io_base + 0x3C, 0x0005);

	    // configure rx_buffer
	    outportb(io_base + 0x44, 0xf | (1 << 7));

	    // enable rec / trans
	    outportb(io_base + 0x37, 0x0c);

	    uint32_t mac_part1 = Ports::inl(io_base + 0x00);
	    uint32_t mac_part2 = Ports::inl(io_base + 0x04);

	    uint8_t mac_addr[6];
	    mac_addr[0] = mac_part1 >> 0;
	    mac_addr[1] = mac_part1 >> 8;
	    mac_addr[2] = mac_part1 >> 16;
	    mac_addr[3] = mac_part1 >> 24;
	    
	    mac_addr[4] = mac_part2 >> 0;
	    mac_addr[5] = mac_part2 >> 8;

	    term.printf("MAC Address: %x:%x:%x:%x:%x:%x\n", 
	    	mac_addr[0],
	    	mac_addr[1],
	    	mac_addr[2],
	    	mac_addr[3],
	    	mac_addr[4],
	    	mac_addr[5]);
	} else {
		term.printf("RTL8139 not discovered on device.\n");
	}
}