using Scripts.Engine;
using System;
using System.Runtime.CompilerServices;

namespace Scripts.Components
{
    public class AudioComponent : Component
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalLoadSound(IntPtr handle, string soundName, bool b3d = true, bool looping = false, bool stream = false);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalUnLoadSound(IntPtr handle, string soundName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalPlay(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalStopChannel(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetVolume(IntPtr handle, float volume);


        public AudioComponent(Actor owner, bool internalCreate = true) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.AudioComponentType);
            }
        }

        public void LoadSound(string soundName, bool b3d = true, bool looping = false, bool stream = false)
        {
            InternalLoadSound(CppInstance, soundName, b3d, looping, stream); 
        }

        public void UnLoadSound(string soundName)
        {
            InternalUnLoadSound(CppInstance, soundName);
        }

        public void Play()
        {
            InternalPlay(CppInstance);
        }

        public void StopChannel()
        {
            InternalStopChannel(CppInstance);
        }

        public void SetVolume(float volume)
        {
            InternalSetVolume(CppInstance, volume);
        }

    }
}