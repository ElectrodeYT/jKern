// jKern arch/arm/paging.h
// Header for paging code

#ifndef PAGING_H
#define PAGING_H

void init_kernel_translation_table();
void map_address(uint32_t translation_table, uint32_t phys, uint32_t virt);
uint32_t create_user_translation_table();

void add_translation_descriptor_block(uint32_t translation_table, uint32_t id, uint32_t output_address);

#endif
