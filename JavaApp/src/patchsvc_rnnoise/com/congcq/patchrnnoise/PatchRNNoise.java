package com.congcq.patchrnnoise;

import java.io.*;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.lang.reflect.InvocationTargetException;
import java.security.ProtectionDomain;

public class PatchRNNoise implements ClassFileTransformer {
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
    ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {
        byte[] transformeredByteCode = classfileBuffer;
        if (className.equals("de/maxhenkel/rnnoise4j/Denoiser")) {
            System.out.println("PatchRNNoise: Replacing class");
            try {
                InputStream inputStream = PatchRNNoise.class.getClassLoader().getResourceAsStream("de/maxhenkel/rnnoise4j/Denoiser.class.patch");
                transformeredByteCode = new byte[inputStream.available()];
                DataInputStream dataInputStream = new DataInputStream(inputStream);
                dataInputStream.readFully(transformeredByteCode);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return transformeredByteCode;
    }

    public static void premain(String args, Instrumentation instrumentation) {
        System.out.println("PatchRNNoise: premain called");
        instrumentation.addTransformer(new PatchRNNoise());
    }
}
