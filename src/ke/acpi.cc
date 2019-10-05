#include <acpi.h>
#include <kernel.h>
#include <system.h>
#include <string.h>

uint32_t 	*SMI_CMD;
uint8_t 	ACPI_ENABLE;
uint8_t 	ACPI_DISABLE;
uint32_t 	*PM1a_CNT;
uint32_t 	*PM1b_CNT;
uint16_t 	SLP_TYPa;
uint16_t 	SLP_TYPb;
uint16_t 	SLP_EN;
uint16_t 	SCI_EN;
uint8_t 	PM1_CNT_LEN;

int32_t* rsdp_scan_mem() {
	uint32_t* ptr = (uint32_t*)RSDP_MEM_MIN;

	for (; (uint32_t)ptr < RSDP_MEM_MAX; *ptr++) {
		RSDPDescriptor20* desc = (RSDPDescriptor20*)ptr;

		if (!strcmpl(desc->firstPart.Signature, "RSD PTR ", 8)) {
			return ptr;
		}
	}

	return NULL;
}

RSDPDescriptor20* rsdp_init(bool debug) {
	uint32_t* ptr = rsdp_scan_mem();

	if (ptr != NULL)
		if (debug) term.printf("[RSDP] Ptr found 0x%x\n", ptr);


	return (RSDPDescriptor20*)ptr;
}

RSDT* rsdt_init(RSDPDescriptor20* dp, bool debug) {
	if (dp == NULL) {
		term.printf("[RSDT] RSDP NULL! ABORTING\n");
		return;
	}

	ACPISDTHeader* dt = (ACPISDTHeader*)dp->firstPart.RsdtAddress;

	if (!rsdt_check(dt)) {
		term.printf("[RSDT] Checksum not valid!\n");
		return;
	}

	RSDT* complete = (RSDT*)dt;

	return complete;
}

bool rsdt_check(ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < tableHeader->Length; i++)
    {
        sum += ((char *) tableHeader)[i];
    }
 
    return sum == 0;
}

FADT* facp_find(RSDT* RootSDT, bool debug)
{
    RSDT* rsdt = RootSDT;
    int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 	FADT* out = NULL;

    for (int i = 0; i < entries; i++)
    {
        ACPISDTHeader* h = (ACPISDTHeader*) rsdt->PointerToOtherSDT[i];
        if (!strcmpl(h->Signature, "FACP", 4)) {
        	if (debug) term.printf("[FADT] Header found at 0x%x\n", h);
            out = (FADT*)h;
        }

    }
 
    return out;
}

void acpi_init() {
	RSDPDescriptor20* rsdp = rsdp_init(true);
	RSDT* rsdt = rsdt_init(rsdp, true);
	FADT* fadt = facp_find(rsdt, true);
	
	if (fadt->SMI_CommandPort != 0) {
		outportb(fadt->SMI_CommandPort, fadt->AcpiEnable);
		term.printf("Enabled ACPI\n");
	}

	uint32_t* dsdt = (uint32_t*)(fadt->Dsdt);

	if (strcmpl((uint8_t*)dsdt, "DSDT", 4) == 0) {
		char* S5Addr = (char*)fadt->Dsdt + 36;
		char* SBAddr = (char*)fadt->Dsdt;
		int dsdt_length = *(dsdt + 0x1) - 36;

		while (0 < dsdt_length--) {
			if (strcmpl(S5Addr, "_S5_", 4) == 0)
				break;

			S5Addr++;
		}

		if (dsdt_length > 0) {
			S5Addr += 5;
            S5Addr += ((*S5Addr &0xC0)>>6) +2;   // calculate PkgLength size

            if (*S5Addr == 0x0A)
            	S5Addr++;   // skip byteprefix
            SLP_TYPa = *(S5Addr)<<10;
            S5Addr++;

            if (*S5Addr == 0x0A)
                S5Addr++;   // skip byteprefix
            SLP_TYPb = *(S5Addr)<<10;

            SMI_CMD = fadt->SMI_CommandPort;

            ACPI_ENABLE = fadt->AcpiEnable;
            ACPI_DISABLE = fadt->AcpiDisable;

            PM1a_CNT = fadt->PM1aControlBlock;
            PM1b_CNT = fadt->PM1bControlBlock;
             
            PM1_CNT_LEN = fadt->PM1ControlLength;

            SLP_EN = 1<<13;
            SCI_EN = 1;
		} else {
			term.printf("S5 not found!\n");
		}

		// note for lesser dunk me haha:
		// find what is \_SB and look around on the pdf
		// perhaps i can find something interesting?
		dsdt_length = *(dsdt + 0x1);

		while (0 < dsdt_length--) {
			if (strcmpl(SBAddr, "_SB", 3) == 0) {
				break;
			}

			SBAddr++;
		}

		if (dsdt_length > 0) {
			term.printf("Found _SB\n");
		} else {
			term.printf("Failed to find _SB\n");
		}

	} else {
		term.printf("DSDT Not supported??\n");
	}
}

void acpi_poweroff() {
	// SCI_EN is set to 1 if acpi shutdown is possible
   	if (SCI_EN == 0)
    	return;

   	// send the shutdown command
   	outportw((unsigned int) PM1a_CNT, SLP_TYPa | SLP_EN );
   	if (PM1b_CNT != 0)
    	outportw((unsigned int) PM1b_CNT, SLP_TYPb | SLP_EN );

   	term.printf("acpi poweroff failed.\n");
}