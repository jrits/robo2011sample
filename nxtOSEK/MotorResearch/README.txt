���[�^�␳�W���v�Z

���O����
1. main.cpp ���J��
2. #define PWM_RESEARCH ��L���ɂ���(�L���ɂȂ��Ă��邱�Ƃ��m�F����)
3. USER_GYRO_OFFSET �̒l�� nxtOSEK/samples_c/helloworld ��p���ēK�X��������B
4. make all ���A�v���O������NXT�{�̂ɃA�b�v���[�h����B

���O�擾
1. NXT�{�̂����̏�ɐQ�����A�v���O���������s����B
2. ���O���擾���邽�߁ABluetooth�ڑ�������B
3. �^�b�`�Z���T�{�^���������A���点��B
4. ���X�ɃX�s�[�h�������铮��������B�I���܂ő҂B
���O��PWM�l�w�莞�_�ɂ����鍶�E���[�^�G���R�[�_�l�ω���(���x)���L�^�����B

���[�^�␳�W�����
�����x / �E���x���v���b�g���Ă݂�B���߂Ă݂�B
�����x - �E���x���v���b�g���Ă݂�B���߂Ă݂�B

�v�Z

���̎��ŎZ�o�ł������H
    if (�����x / �E���x > 1.0) {
        ���W�� = �E���x / �����x;
        �E�W�� = 1.0;
    }
    else {
        ���W�� = 1.0;
        �E�W�� = �����x / �E���x;
    }

����
1. main.cpp ���J��
2. #define PWM_RESEARCH ���R�����g�A�E�g���A#define PWM_VERIFICATION ��L���ɂ���B
3. �v�Z�����W���� float PWM_L ����� float PWM_R �ɃZ�b�g����B
4. make all ���A�v���O������NXT�{�̂ɃA�b�v���[�h����B
5. ���点�Ă݂�B�␳�O���^����������ΐ����B
