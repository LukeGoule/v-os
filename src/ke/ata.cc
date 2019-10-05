#include <kernel.h>
#include <ata.h>
#include <terminal.h>

#define orange() term.SetColour(0xFF7F50);
#define white() term.SetColour(0xFFFFFF);
#define INFO orange() term.printf("\tINFO"); white()

static struct ata_device ata_primary_master 	= {.io_base = 0x1F0, .control = 0x3F6, .slave = 0};
static struct ata_device ata_primary_slave 		= {.io_base = 0x1F0, .control = 0x3F6, .slave = 1};
static struct ata_device ata_secondary_master 	= {.io_base = 0x170, .control = 0x376, .slave = 0};
static struct ata_device ata_secondary_slave 	= {.io_base = 0x170, .control = 0x376, .slave = 1};

ata_device ata_primary(bool master_slave) {
	return (master_slave ? ata_primary_master : ata_primary_slave);
}

void ata_io_wait(struct ata_device * dev) {
	Ports::inb(dev->io_base + ATA_REG_ALTSTATUS);
	Ports::inb(dev->io_base + ATA_REG_ALTSTATUS);
	Ports::inb(dev->io_base + ATA_REG_ALTSTATUS);
	Ports::inb(dev->io_base + ATA_REG_ALTSTATUS);
}

int ata_status_wait(struct ata_device * dev, int timeout) {
	int status;
	if (timeout > 0) {
		int i = 0;
		while ((status = Ports::inb(dev->io_base + ATA_REG_STATUS)) & ATA_SR_BSY && (i < timeout)) i++; 
	} else {
		while ((status = Ports::inb(dev->io_base + ATA_REG_STATUS)) & ATA_SR_BSY);
	}
	return status;
}

int ata_wait(struct ata_device * dev, int advanced) {
	uint8_t status = 0;
	ata_io_wait(dev);

	status = ata_status_wait(dev, -1);

	if (advanced) {
		status = Ports::inb(dev->io_base + ATA_REG_STATUS);
		if (status 	& ATA_SR_ERR) return -1;
		if (status 	& ATA_SR_DF) return -1;
		if (status 	& ATA_SR_DRQ) return -1;
	}

	return 0;
}

void ata_init_device(struct ata_device * dev) {
	term.printf("Initialising IDE dev on %d\n", dev->io_base);

	Ports::outb(dev->io_base + 1, 1);
	Ports::outb(dev->control, 0);
	Ports::outb(dev->io_base + ATA_REG_HDDEVSEL, 0xA0 | dev->slave << 4);
	ata_io_wait(dev);

	Ports::outb(dev->io_base + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

	int status = Ports::inb(dev->io_base + ATA_REG_COMMAND);
	INFO term.printf(" - Device status: 0x%x\n", status);

	if (status == 0) {
		return;
	}

	ata_wait(dev, 0);

	uint16_t* buf = (uint16_t*)&dev->identify;
	for (int i = 0; i < 256; i++) {
		buf[i] = Ports::inw(dev->io_base);
	}

	ata_params* test = (ata_params*)&dev->identify;

	uint8_t* ptr = (uint8_t*)&dev->identify.model;
	for (int i = 0; i < 39; i+=2) {
		uint8_t tmp = ptr[i+1];
		ptr[i+1] = ptr[i];
		ptr[i] = tmp;
	}
	
	INFO term.printf(" - Device model: \"%s\"\n", test->model);
	INFO term.printf(" - Serial code:  \"%s\"\n", test->serial);
	INFO term.printf(" - Firmware:     \"%s\"\n", test->revision);
	INFO term.printf(" - Cyclinders:   %d\n", (uint32_t)test->cylinders);
	INFO term.printf(" - Heads:        %d\n", (uint32_t)test->heads);	
	INFO term.printf(" - Sectors(48):  0x%x (%d bytes)\n", (uint32_t)dev->identify.sectors_48, dev->identify.sectors_48 * 512);
	INFO term.printf(" - Sectors(28):  0x%x (%d bytes)\n", (uint32_t)dev->identify.sectors_28, dev->identify.sectors_28 * 512);
	INFO term.printf(" - Capabilities:\n");
	if (test->capabilities1 & ATA_SUPPORT_DMA) 		{ term.printf("\t\t- DMA\n"); } 
	if (test->capabilities1 & ATA_SUPPORT_LBA) 		{ term.printf("\t\t- LBA\n"); }
	if (test->capabilities1 & ATA_SUPPORT_IORDY) 	{ term.printf("\t\t- IORDY\n"); } 
	if (test->capabilities1 & ATA_SUPPORT_IORDYDIS) { term.printf("\t\t- IORDYDIS\n"); }
	if (test->capabilities1 & ATA_SUPPORT_OVERLAP) 	{ term.printf("\t\t- OVERLAP\n"); }
	INFO term.printf(" - Queue Length: %d\n", ATA_QUEUE_LEN(test->queue));
	INFO term.printf(" - Version: %d:%d\n", test->version_major, test->version_minor);

	Ports::outb(dev->io_base + ATA_REG_COMMAND, 0x02);
}