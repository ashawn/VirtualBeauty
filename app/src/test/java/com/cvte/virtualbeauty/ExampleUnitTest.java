package com.cvte.virtualbeauty;

import org.junit.Test;
import org.reactivestreams.Publisher;

import io.reactivex.BackpressureStrategy;
import io.reactivex.Flowable;
import io.reactivex.FlowableEmitter;
import io.reactivex.functions.Function;

import static org.junit.Assert.assertEquals;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {

    interface Callback {
        void done(String result);
    }

    @Test
    public void addition_isCorrect() throws Exception {
        assertEquals(4, 2 + 2);
    }

    @Test
    public void test() {
        Flowable.range(0, 10)
                .map(i -> {
                    if (i%3==0) return "failed handle "+i;
                    return "handle " + i;
                })
                .flatMap(new Function<String, Publisher<String>>() {
                    @Override
                    public Publisher<String> apply(String s) throws Exception {
                        if(s.startsWith("failed")) return Flowable.create((FlowableEmitter<String> emitter)->{
                            asyncMethod(s, result -> {
                                emitter.onNext(result);
                                emitter.onComplete();
                            });
                        }, BackpressureStrategy.BUFFER);
                        return Flowable.just(s);
                    }
                })
                .subscribe(this::log);
    }

    private void asyncMethod(String content, final Callback cb) {
        cb.done("[async]" + content);
    }

    private void log(String msg) {
        System.out.printf(msg + "\n");
    }
}