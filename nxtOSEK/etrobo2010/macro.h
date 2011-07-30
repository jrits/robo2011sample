#ifndef __MACRO_H__
#define __MACRO_H__

/**
 *	�z��v�f���擾
 *
 *	�z��̗v�f�����擾����B
 *
 * @param[in] array �z�񖼁B�|�C���^�ł͂Ȃ����ƁB
 * @return �z��̗v�f��
 */
#define COUNTOF(array) (sizeof(array) / sizeof((array)[0]))

/**
 * ��Βl���擾����B
 *
 * @param[in] value �l
 * @return ��Βl
 */
#define ABS(value) ((value) < 0 ? -(value) : (value))

/**
 * �������擾����B
 *
 * @param[in] value �l
 * @return ����
 */
#define SIGN(value) ((value) > 0 ? 1 : -1)

/**
 * �傫�����̒l���擾����B
 *
 * @param[in] a �l�P
 * @param[in] b �l�Q
 * @return �傫�����̒l
 */
#define MAX(a,b) (((a)>(b))?(a):(b))

/**
 * ���������̒l���擾����B
 *
 * @param[in] a �l�P
 * @param[in] b �l�Q
 * @return ���������̒l
 */
#define MIN(a,b) (((a)<(b))?(a):(b))

#endif // __MACRO_H__
