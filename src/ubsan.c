/*
КТО ПРОЧИТАЛ ТОТ ХУЕСОС, ПИДОРАС, ШЛЮХА
ИДИТЕ НАХУЙ 
ХУЙ
ХУЙ
ХУЙ
ХУЙ
СОСИТЕ ХУЙ
/*
#include <ubsan.h>
#include <Panic.h>
#include <string.h>
#include <types.h>
#include <print.h>

#define is_aligned(value, alignment) !(value & (alignment - 1))
#define VALUE_LENGTH 40

const char *Type_Check_Kinds[] = {
    "load of",
    "store to",
    "reference binding to",
    "member access within",
    "member call on",
    "constructor call on",
    "downcast of",
    "downcast of",
    "upcast of",
    "cast to virtual base of",
    "_Nonnull binding to",
};

enum {
	type_kind_int = 0,
	type_kind_float = 1,
	type_unknown = 0xffff
};

static unsigned type_bit_width(struct type_descriptor *type)
{
	return 1 << (type->info >> 1);
}

static int type_is_int(struct type_descriptor *type)
{
	return type->kind == type_kind_int;
}

static int type_is_signed(struct type_descriptor *type)
{
	return  type->info & 1;
}

static bool is_inline_int(struct type_descriptor *type)
{
	unsigned inline_bits = sizeof(unsigned long)*8;
	unsigned bits = type_bit_width(type);

	return bits <= inline_bits;
}

static s_max get_signed_val(struct type_descriptor *type, unsigned long val)
{
	if (is_inline_int(type)) {
		unsigned extra_bits = sizeof(s_max)*8 - type_bit_width(type);
		return ((s_max)val) << extra_bits >> extra_bits;
	}

	if (type_bit_width(type) == 64)
		return *(int64_t *)val;

	return *(s_max *)val;
}

static u_max get_unsigned_val(struct type_descriptor *type, unsigned long val)
{
	if (is_inline_int(type))
		return val;

	if (type_bit_width(type) == 64)
		return *(uint64_t *)val;

	return *(u_max *)val;
}

static int val_is_negative(struct type_descriptor *type, unsigned long val)
{
        return type_is_signed(type) && get_signed_val(type, val) < 0;
}

static void log_location(struct source_location *location) {
    printf("\nfile: %s\n	line: %i\n		column: %i\n",
         location->file, location->line, location->column);
}

static void val_to_string(char *str, size_t size, struct type_descriptor *type,
	unsigned long value)
{
	if (type_is_int(type)) {
		if (type_bit_width(type) == 128) {
		    u_max val = get_unsigned_val(type, value);

			sprintf(str, "0x%08x%08x%08x%08x",
				(u32)(val >> 96),
				(u32)(val >> 64),
				(u32)(val >> 32),
				(u32)(val));

		} else if (type_is_signed(type)) {
			sprintf(str, "%d",
				(int64_t)get_signed_val(type, value));
		} else {
			sprintf(str, "%iu",
				(uint64_t)get_unsigned_val(type, value));
		}
	}
}

static void handle_overflow(struct overflow_data *data, unsigned long lhs,
			unsigned long rhs, char op)
{

	struct type_descriptor *type = data->type;
	char lhs_val_str[VALUE_LENGTH];
	char rhs_val_str[VALUE_LENGTH];

	val_to_string(lhs_val_str, sizeof(lhs_val_str), type, lhs);
	val_to_string(rhs_val_str, sizeof(rhs_val_str), type, rhs);
	printf("%s integer overflow:\n",
		type_is_signed(type) ? "signed" : "unsigned");
	printf("%s %c %s cannot be represented in type %s\n",
		lhs_val_str,
		op,
		rhs_val_str,
		type->name);

	_panic("Ubsan check failed", "unknown", data->location.file, data->location.line);
}

void __ubsan_handle_add_overflow(struct overflow_data *data,
				unsigned long lhs,
				unsigned long rhs)
{

	handle_overflow(data, lhs, rhs, '+');
}

void __ubsan_handle_sub_overflow(struct overflow_data *data,
				unsigned long lhs,
				unsigned long rhs)
{
	handle_overflow(data, lhs, rhs, '-');
}

void __ubsan_handle_mul_overflow(struct overflow_data *data,
				unsigned long lhs,
				unsigned long rhs)
{
	handle_overflow(data, lhs, rhs, '*');
}

void __ubsan_handle_negate_overflow(struct overflow_data *data,
				unsigned long old_val)
{
	char old_val_str[VALUE_LENGTH];

	val_to_string(old_val_str, sizeof(old_val_str), data->type, old_val);

	printf("negation of %s cannot be represented in type %s:\n",
		old_val_str, data->type->name);

	_panic("Ubsan check failed", "unknown", data->location.file, data->location.line);

}

void __ubsan_handle_divrem_overflow(struct overflow_data *data,
				unsigned long lhs,
				unsigned long rhs)
{
	char rhs_val_str[VALUE_LENGTH];

	val_to_string(rhs_val_str, sizeof(rhs_val_str), data->type, rhs);

	if (type_is_signed(data->type) && get_signed_val(data->type, rhs) == -1)
		printf("division of %s by -1 cannot be represented in type %s\n",
			rhs_val_str, data->type->name);
	else
		printf("division by zero\n");

	_panic("Ubsan check failed", "unknown", data->location.file, data->location.line);
}

static void handle_null_ptr_deref(struct type_mismatch_data_common *data)
{
	printf("%s null pointer of type %s\n",
		Type_Check_Kinds[data->type_check_kind],
		data->type->name);

	_panic("Ubsan check failed", "unknown", data->location->file, data->location->line);
}

void __ubsan_handle_type_mismatch(struct type_mismatch_data *type_mismatch,
                                  uintptr_t pointer) {
    struct source_location *location = &type_mismatch->location;
    if (pointer == 0) {
        printf("Null pointer access");
    } else if (type_mismatch->log_alignment != 0 &&
               is_aligned(pointer, type_mismatch->log_alignment)) {
        // Most useful on architectures with stricter memory alignment requirements, like ARM.
        printf("Unaligned memory access");
    } else {
        printf("Insufficient size");
        printf("%s address %p with insufficient space for object of type %s\n",
             Type_Check_Kinds[type_mismatch->type_check_kind], (void *)pointer,
             type_mismatch->type->name);
    }
    log_location(location);

    _panic("Ubsan check failed", "unknown", location->file, location->line);
}

void __ubsan_handle_type_mismatch_v1(struct type_mismatch_data_v1 *data,
				unsigned long ptr)
{
	struct type_mismatch_data d = {
		.location = data->location,
		.type = data->type,
		.log_alignment = 1ul << data->log_alignment,
		.type_check_kind = data->type_check_kind,
	};

	__ubsan_handle_type_mismatch(&d, ptr);
}

void __ubsan_handle_out_of_bounds(struct out_of_bounds_data *data, unsigned long index)
{

  //char index_str[VALUE_LENGTH];

  printf("index is out of range for type %s\n", data->array_type->name);

  _panic("Ubsan check failed", "unknown", data->location.file, data->location.line);

        //val_to_string(index_str, sizeof(index_str), data->index_type, index);
}

void __ubsan_handle_pointer_overflow(struct pointer_overflow_data *data,
				unsigned long base, unsigned long result)
{
	printf("\npointer overflow:\n");

	if (((long)base >= 0) == ((long)result >= 0))
		printf("%s of unsigned offset to %p overflowed to %p\n",
			base > result ? "addition" : "subtraction",
			(void *)base, (void *)result);
	else
		printf("pointer index expression with base %p overflowed to %p\n",
			(void *)base, (void *)result);

	_panic("UBSan check failed", "unknown", data->location.file, data->location.line);
}

void __ubsan_handle_shift_out_of_bounds(struct shift_out_of_bounds_data *data,
					unsigned long lhs, unsigned long rhs)
{
	struct type_descriptor *rhs_type = data->rhs_type;
	struct type_descriptor *lhs_type = data->lhs_type;
	char rhs_str[VALUE_LENGTH];
	char lhs_str[VALUE_LENGTH];

	val_to_string(rhs_str, sizeof(rhs_str), rhs_type, rhs);
	val_to_string(lhs_str, sizeof(lhs_str), lhs_type, lhs);

	if (val_is_negative(rhs_type, rhs))
		printf("shift exponent %s is negative\n", rhs_str);

	else if (get_unsigned_val(rhs_type, rhs) >=
		type_bit_width(lhs_type))
		printf("shift exponent %s is too large for %u-bit type %s\n",
			rhs_str,
			type_bit_width(lhs_type),
			lhs_type->name);
	else if (val_is_negative(lhs_type, lhs))
		printf("left shift of negative value %s\n",
			lhs_str);
	else
		printf("left shift of %s by %s places cannot be"
			" represented in type %s\n",
			lhs_str, rhs_str,
			lhs_type->name);

	_panic("Ubsan check failed", "unknown", data->location.file, data->location.line);
}

void __ubsan_handle_load_invalid_value(struct invalid_value_data *data,
				unsigned long val)
{
	char val_str[VALUE_LENGTH];

	val_to_string(val_str, sizeof(val_str), data->type, val);

	printf("load of value %s is not a valid value for type %s\n",
		val_str, data->type->name);

	_panic("Ubsan check failed", "unknown", data->location.file, data->location.line);
}*/
// Eblan